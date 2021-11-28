#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bn_s bn;

enum bn_codes { BN_OK, BN_NULL_OBJECT, BN_NO_MEMORY, BN_DIVIDE_BY_ZERO, };

struct bn_s {
	uint32_t* body;
	size_t size;
	size_t capacity;
	int8_t sign;
};

extern bn *bn_new();
extern bn *bn_init(const bn* const orig);
extern int init_string(bn* t, const char* init_string);
extern int bn_init_string_radix(bn* t, const char* init_string, int radix);
extern int bn_init_int(bn* t, int init_int);
extern int bn_delete(bn* t);
extern int bn_add_to(bn* t, bn* const right);
extern int bn_sub_to(bn* t, bn* const right);
extern int bn_neg(bn* t);

static int _check_bn_NULL(const bn* x, const bn* y) {
	if (x == NULL || y == NULL) return 1;
	return 0;
}

static int _check_body_NULL(const bn* x, const bn* y) {
	if (x->body == NULL || y->body == NULL) return 1;
	return 0;
}

static void _reverse(char* str, size_t size) {
	uint8_t tmp;
	for (size_t i = 0, j = size-1; i < size / 2; ++i, --j) {
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}
}

static void _inc_size(bn* r) { //incremet size
	r->size += 1;
	if (r->size == r->capacity) {
		r->capacity <<= 1;
		r->body = (uint32_t *)realloc(r->body, sizeof(uint32_t) * r->capacity);
		memset(r->body + r->size, 0, (r->capacity - r->size) * sizeof(uint32_t));
	}
}

static void _check_is_zero(bn* t) { //Making zeros of non-zeros bn
	if (t->size != 0 && t->body != NULL) {
		memset(t->body, 0, sizeof(uint32_t) * t->capacity);
		t->size = 0;
		t->sign = 1;
	}
}

static int _check_capacity(bn* t) { //Checking if we have more than we really need
	if (t->size <= t->capacity >> 2) {
		t->capacity >>= 1;
		t->body = (uint32_t *)realloc(t->body, sizeof(uint32_t) * t->capacity);
		if (t->body == NULL) return BN_NO_MEMORY;
	}
	return BN_OK;
}

static int _set_size(bn* left, bn* const right) { //Set left size equals to right
	if (left->capacity < right->capacity) {
		left->capacity = right->capacity;
		left->body = (uint32_t *)realloc(left->body, left->capacity * sizeof(uint32_t));
		if (left->body == NULL) return BN_NO_MEMORY;
		memset(left->body + left->size, 0, (left->capacity - left->size) * sizeof(uint32_t));
	}
	if (right->capacity < left->capacity) {
		right->capacity = left->capacity;
		right->body = (uint32_t *)realloc(right->body, right->capacity * sizeof(uint32_t));
		if (right->body == NULL) return BN_NO_MEMORY;
		memset(right->body + right->size, 0, (right->capacity - right->size) * sizeof(uint32_t));
	}
	if (left->size < right->size) {
		left->size = right->size;
	}
	return BN_OK;
}

extern bn *bn_new() {
	bn* r = (bn *)malloc(sizeof(bn));
	if (r == NULL) return NULL;
	r->size = 1;
	r->capacity = 2;
	r->sign = 1;
	r->body = (uint32_t *)calloc(sizeof(uint32_t) * r->capacity, 0);
	if (r->body == NULL) {
		free(r);
		return NULL;
	}
	return r;
}

extern bn *bn_init(const bn* const orig) {
	if (orig == NULL) return NULL;
	bn* r = (bn *)malloc(sizeof(bn));
	if (r == NULL) return NULL;
	r->size = orig->size;
	r->capacity = orig->capacity;
	r->sign = orig->sign;
	r->body = (uint32_t *)malloc(sizeof(uint32_t ) * r->capacity);
	if (r->body == NULL) {
		free(r);
		return NULL;
	}
	memcpy(r->body, orig->body, r->size * sizeof(uint32_t));
	return r;
}

extern int bn_init_string(bn* t, const char* init_string) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->body == NULL) return BN_NO_MEMORY;
	_check_is_zero(t);
	const size_t BUFFSIZE = 9;
	char chunk[BUFFSIZE + 1];
	memset(chunk, 0, (BUFFSIZE + 1) * sizeof(char));
	size_t length = strlen(init_string);
	size_t index = 0, cur = 0;
	if (length == 0) {
		return BN_OK;
	}
	for (size_t i = length - 1; i != 0; --i) {
		chunk[index++] = init_string[i];
		if (index == BUFFSIZE) {
			_reverse(chunk, BUFFSIZE);
			//chunk[index+1] = '\0';
			t->body[cur++] = atoi(chunk);
			_inc_size(t);
			index = 0;
		}
	}
	if (init_string[0] == '-') {
		t->sign = -1;
	}
	else {
		chunk[index++] = init_string[0];
	}
	if (index != 0) {
		chunk[index] = '\0';
		_reverse(chunk, index);
		t->body[cur++] = atoi(chunk);
		_inc_size(t);
	}
	_check_capacity(t);
	return BN_OK;
}

extern int bn_init_string_radix(bn* t, const char* init_string, int radix) {

}

extern int bn_init_int(bn* t, int init_int) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->body == NULL) return BN_NO_MEMORY;
	_check_is_zero(t);
	if (init_int < 0) {
		t->sign = -1;
		init_int = -init_int;
	}
	t->body[0] = init_int;
	_inc_size(t);
	return BN_OK;
}

extern int bn_delete(bn* t) {
	if (t == NULL) return BN_NULL_OBJECT;
	t->size = 0;
	t->sign = 0;
	if (t->body == NULL) {
		free(t);
		return BN_NO_MEMORY;
	}
	free(t->body);
	free(t);
	return BN_OK;
}

// +=
extern int bn_add_to(bn* t, bn* const right) {
	if (_check_bn_NULL(t, right)) return BN_NULL_OBJECT;
	if (_check_body_NULL(t, right)) return BN_NO_MEMORY;
	if (t->sign != right->sign) {
		bn_neg(right);
		int ERROR = bn_sub_to(t, right);
		bn_neg(right);
		return ERROR;
	}
	if (_set_size(t, right) != BN_OK) {
		return BN_NO_MEMORY;
	}
	uint8_t F = 0;
	for (size_t i = 0; i < t->size; ++i) {
		uint64_t tmp = t->body[i] + right->body[i] + F;
		t->body[i] = tmp % 1000000000;
		F = tmp / 1000000000;
	}
	if (F) {
		t->body[t->size] = F;
		_inc_size(t);
	}
	_check_capacity(t);
	return BN_OK;
}

// -=
extern int bn_sub_to(bn *t, bn* const right) {
	if (_check_bn_NULL(t, right)) return BN_NULL_OBJECT;
	if (_check_body_NULL(t, right)) return BN_NO_MEMORY;
	if (t->sign != right->sign) {
		bn_neg(right);
		int ERROR = bn_add_to(t, right);
		bn_neg(right);
		return ERROR;
	}
	uint8_t F = 0;
	//if (_set_size(t, right) != BN_OK) return BN_NO_MEMORY;
	for (ssize_t i = 0; i < t->size; ++i) {
		uint64_t tmp = (t->body[i] + 1000000000) - (right->body[i] + F);
		t->body[i] = tmp % 1000000000;
		F = (tmp >= 1000000000) ? 0 : 1;
	}
	return BN_OK;
}

// *=
extern int bn_mul_to(bn *t, bn* const right) {

}

// /=
extern int bn_div_to(bn *t, bn* const right) {

}

// %=
extern int bn_mod_to(bn *t, bn* const right) {

}

// Возвести число в степень degree
extern int bn_pow_to(bn *t, int degree) {

}

// Извлечь корень степени reciprocal из BN
extern int bn_root_to(bn *t, int reciprocal) {

}

// = l + r
extern bn* bn_add(bn* const left, bn* const right) {

}

// = l - r
extern bn* bn_sub(bn* const left, bn* const right) {
	if (left == NULL || right == NULL) return NULL;
	if (left->body == NULL || right->body == NULL) return NULL;
	bn* t = bn_new();
	return t;
}

// = l * r
extern bn* bn_mul(bn* const left, bn* const right) {

}

// = l / r
extern bn* bn_div(bn* const left, bn* const right) {

}

// = l % r
extern bn* bn_mod(bn* const left, bn* const right) {
	
}

// Выдать представление BN в системе счисления radix в виде строки
// Строку после использования потребуется удалить.
const char *bn_to_string(const bn* const t, int radix) {

}

// Если левое меньше, вернуть <0; если равны, вернуть 0; иначе  >0
extern int bn_cmp(const bn* const left, const bn* const right) {
	if (_check_bn_NULL(left, right)) return BN_NULL_OBJECT;
	if (_check_body_NULL(left, right)) return BN_NO_MEMORY;
	if (left->sign == right->sign) {
		if (left->size != right->size) return (left->size > right->size) ? 10 : -10;
		for (ssize_t i = left->size - 1; i != -1; --i) {
			if (left->body[i] < right->body[i]) return -10;
			if (left->body[i] > right->body[i]) return 10;
		}
	}
	else return (left->sign > right->sign) ? 10 : -10;
	return 0;
}

// Изменить знак на противоположный
extern int bn_neg(bn *t) {
	if (t == NULL) return BN_NULL_OBJECT;
	t->sign = -t->sign;
	return BN_OK;
}

// Взять модуль
extern int bn_abs(bn *t) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->sign == 1) return BN_OK;
	t->sign = 1;
	return BN_OK;
}

//-1 если t<0; 0 если t = 0, 1 если t>0
extern int bn_sign(bn const *t) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->sign == -1) return -1;
	if (t->sign == 1) return 1;
	return -1;
}

extern void print_bn(const bn* const t) {
	if (t->size == 0) {
		printf("null_object\n");
		return;
	}
	if (t->sign == -1) printf("-");
	ssize_t i = t->size - 1;
	for (; i > 0; --i) {
		if (t->body[i] != 0) break;
	}
	printf("%u", t->body[i]);
	for (--i; i != -1; --i) {
		printf("%.9u", t->body[i]);
	}
	putchar('\n');
}

int main() {
	bn* x = bn_new();
	bn* y = bn_new();
	bn_init_string(x, "9999999999999999999999");
	bn_init_string(y, "1");
	bn* z = bn_init(y);

	print_bn(x);
	print_bn(y);
	print_bn(z);
	printf("______________________\n");

	bn_add_to(x, y);

	print_bn(x);
	print_bn(y);
	print_bn(z);


	bn_delete(x);
	bn_delete(y);
	bn_delete(z);
}
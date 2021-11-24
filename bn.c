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

static void _reverse(char* str, size_t size) {
	uint8_t tmp;
	for (size_t i = 0, j = size-1; i < size / 2; ++i, --j) {
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}
}

static void _inc_size(bn* r) {
	r->size += 1;
	if (r->size == r->capacity) {
		r->capacity <<= 1;
		r->body = (uint32_t *)realloc(r->body, sizeof(uint32_t) * r->capacity);
		memset(r->body + r->size, 0, (r->capacity - r->size) * sizeof(uint32_t));
	}
}

static void _check_is_zero(bn* t) {
	if (t->size != 0 && t->body != NULL) {
		memset(t->body, 0, sizeof(uint32_t) * t->size);
		t->size = 0;
		t->sign = 1;
	}
}

static void _check_capacity(bn* t) {
	if (t->size <= t->capacity >> 2) {
		t->capacity >>= 1;
		t->body = (uint32_t *)realloc(t->body, sizeof(uint32_t) * t->capacity);
	}
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
	memcpy(r->body, orig->body, r->size);
	return r;
}

extern int bn_init_string(bn* t, const char* init_string) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->body == NULL) return BN_NO_MEMORY;
	const size_t BUFFSIZE = 9;
	char chunk[BUFFSIZE + 1];
	memset(chunk, 0, (BUFFSIZE + 1) * sizeof(char));
	size_t length = strlen(init_string);
	size_t index = 0, cur = 0;
	for (size_t i = length - 1; i != 0; --i) {
		chunk[index++] = init_string[i];
		if (index == BUFFSIZE) {
			_reverse(chunk, BUFFSIZE);
			chunk[index+1] = '\0';
			t->body[cur++] = atoi(chunk);
			_inc_size(t);
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

}

// -=
extern int bn_sub_to(bn *t, bn const *right) {

}

// *=
extern int bn_mul_to(bn *t, bn const *right) {

}

// /=
extern int bn_div_to(bn *t, bn const *right) {

}

// %=
extern int bn_mod_to(bn *t, bn const *right) {

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
const char *bn_to_string(bn const *t, int radix) {

}

// Если левое меньше, вернуть <0; если равны, вернуть 0; иначе  >0
int bn_cmp(const bn* const left, const bn* const right) {
}

// Изменить знак на противоположный
int bn_neg(bn *t) {
	if (t == NULL) return BN_NULL_OBJECT;
	t->sign = -t->sign;
	return BN_OK;
}

// Взять модуль
int bn_abs(bn *t) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->sign == 1) return BN_OK;
	t->sign = 1;
	return BN_OK;
}

//-1 если t<0; 0 если t = 0, 1 если t>0
int bn_sign(bn const *t) {
	if (t == NULL) return BN_NULL_OBJECT;
	if (t->sign == -1) return -1;
	if (t->sign == 1) return 1;
	return -1;
}

int main() {
	bn *x = bn_new();
	bn *y = bn_new();

	printf("I am on line 230\n");

	bn_init_string(x, "9999999999999999999999999");
	bn_init_string(y, "1");
	printf("I HAVE A PROBLEM!\n");
	bn_delete(x);
	bn_delete(y);
}
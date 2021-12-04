//#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class BigInteger{ //1e9
public:
	BigInteger() {
		sign = 1;
		size = 0;
	}

	BigInteger(const BigInteger& bi1) {
		if (this == &bi1) {
			return;
		}
		if (!size) {
			body.clear();
		}
		size = bi1.size;
		sign = bi1.sign;
		for (uint32_t i = 0; i < size; i++) body.push_back(bi1.body[i]);
	}

	BigInteger(const int t) {
		size = 0;
		sign = 1;
		int copy = t;
		if (t < 0) {
			sign = -1;
			copy = -t;
		}
		if (copy > RADIX) {
			body.push_back(t % RADIX);
			body.push_back(t / RADIX);
			size += 2;
		}
		else {
			body.push_back(copy);
			++size;
		}
	}

	~BigInteger() {}

	bool operator==(const BigInteger& bi1) const {
		if (size != bi1.size || sign != bi1.sign) return false;
		for (uint32_t i = 0; i < size; ++i) {
			if (body[i] != bi1.body[i]) return false;
		}
		return true;
	}

	bool operator!=(const BigInteger& bi1) const {
		return !(*this == bi1);
	}

	bool operator>(const BigInteger& bi1) const {
		if (sign < bi1.sign) return false;
		if (sign == bi1.sign) {
			if (size != bi1.size) return (size > bi1.size);
			for (ssize_t i = size-1; i != -1; ++i) {
				if (body[i] < bi1.body[i]) return false;
			}
		}
		return (*this != bi1);
	}

	bool operator<(const BigInteger& bi1) const {
		return !(*this > bi1);
	}

	bool operator>=(const BigInteger& bi1) const {
		return ((*this > bi1) || (*this == bi1));
	}

	bool operator<=(const BigInteger& bi1) const {
		return ((*this < bi1) || (*this == bi1));
	}

	bool modbi(const BigInteger& bi1) {
		if (size < bi1.size) return false;
		if (size > bi1.size) return true;
		for (ssize_t i = size - 1; i != -1; --i) {
			if (body[i] < bi1.body[i]) return false;
		}
		return true;
	}

	bool check_zero() const {
		if (size == 1 && body[0] == 0) return true;
		return false;
	}

	BigInteger& operator++() {
		if (sign == -1) {
			if (size == 1 && body[0] == 1) {
				sign = 1;
				body[0] = 0;
				return *this;
			}
			*this += 1;
			return *this;
		}
		if (body[0] % 10 == 9) {
			uint32_t index = 0;
			for (; index < size && body[index] == RADIX - 1; ++index) body[index] = 0;
			if (index == size) {
				body.push_back(1);
				++size;
				return *this;
			}
			++body[index];
			return *this;
		}
		++body[0];
		return *this;
	}

    BigInteger& operator--() { //Yes^ it's stupid, but i'm tired...
		*this -= 1;
		return *this;
	}

	BigInteger& operator+=(const BigInteger& bi1) { 
		if (sign != bi1.sign) {
			BigInteger t = bi1;
			t.sign = -t.sign;
			return (*this -= t);
		}
		uint32_t min_size = size > bi1.size ? bi1.size : size;
		uint32_t tmp, CarryFlag = 0;
		uint32_t i = 0;
		for (; i < min_size; ++i) {
			tmp = body[i] + bi1.body[i] + CarryFlag;
			body[i] = tmp % RADIX;
			CarryFlag = tmp / RADIX;
		}
		for (; i < size; ++i) {
			tmp = body[i] + CarryFlag;
			body[i] = tmp % RADIX;
			CarryFlag = tmp / RADIX;
		}
		for (; i < bi1.size; ++i) {
			tmp = bi1.body[i] + CarryFlag;
			body.push_back(tmp % RADIX);
			++size;
			CarryFlag = tmp / RADIX;
		}
		if (CarryFlag) {
			body.push_back(CarryFlag % RADIX);
			++size;
		}
		return *this;
	}

	BigInteger& operator-=(const BigInteger& bi1) {
		if (sign != bi1.sign) {
			BigInteger copy = bi1;
			copy.sign = -copy.sign;
			return (*this += copy);
		}
		BigInteger copy = (modbi(bi1) ? *this : bi1);
		BigInteger copy_min = (modbi(bi1) ? bi1 : *this);
		if (!modbi(bi1) && sign == bi1.sign) copy.sign = -copy.sign;
		uint32_t max_size = (size >= bi1.size ? size : bi1.size);
		uint32_t tmp, CarryFlag = 0;
		for (uint32_t index = 0; index < max_size; ++index) {
			tmp = RADIX + (index < copy.size) * copy.body[index % copy.size] - (index < copy_min.size) * copy_min.body[index % copy_min.size] - CarryFlag;
			copy.body[index] = tmp % RADIX;
			CarryFlag = (tmp >= RADIX ? 0 : 1);
		}
		*this = copy;
		if (check_zero()) sign = 1;
		/*for (ssize_t i = size - 1; body[i] == 0; -- i) {
			body.pop_back();
			--size;
		}*/
		return *this;
	}

	BigInteger& operator*=(const BigInteger& bi1) {
		if (bi1.sign == -1) sign = -sign;
		BigInteger copy_max = (modbi(bi1) ? *this : bi1);
		BigInteger copy_min = (modbi(bi1) ? bi1 : *this);
		BigInteger ans;
		for (uint32_t index = 0; index < copy_max.size + copy_min.size; ++index) {
			ans.body.push_back(0);
			++ans.size;
		}
		ans.sign = sign;
		size_t tmp;
		uint32_t CarryFlag = 0;
		for (uint32_t deg = 0; deg < ans.size; ++deg) {
			tmp = 0;
			for (uint32_t j = (deg > copy_min.size - 1 ? copy_min.size - 1 : deg), i = deg - j; i < copy_max.size && j < copy_min.size; ++i, --j) {
				tmp += (static_cast<size_t>(copy_max.body[i]) * static_cast<size_t>(copy_min.body[j]));
			}
			tmp += static_cast<size_t>(CarryFlag);
			ans.body[deg] = tmp % static_cast<size_t>(RADIX);
			CarryFlag = tmp / static_cast<size_t>(RADIX);
		}
		if (CarryFlag != 0) {
			ans.body.push_back(CarryFlag  % RADIX);
			++ans.size;
		}
		*this = ans;
		if (check_zero()) sign = 1;
		return *this;
	}

	BigInteger operator+(const BigInteger& bi1) const {
		BigInteger copy = *this;
		copy += bi1;
		return copy;
	}

	BigInteger operator*(const BigInteger& bi1) const {
		BigInteger copy = *this;
		copy *= bi1;
		return copy;
	}

	BigInteger& operator/=(const BigInteger& bi1) {
		BigInteger ans = 0;
		while(*this - (ans + 1) * bi1 > 0) ans += 1;
		*this = ans;
		return *this;
		
	}

	BigInteger operator-(const BigInteger& bi1) const {
		BigInteger copy = *this;
		copy -= bi1;
		return copy;
	}

	friend std::istream& operator>>(std::istream& input, BigInteger& bi1);

	friend std::ostream& operator<<(std::ostream& output, const BigInteger& bi1);

private:
	std::vector<uint32_t> body;
	uint32_t size;
	int8_t sign;
	static const uint32_t RADIX = 1000000000;
};

void _reverse(char* buf, const int size) {
	char tmp;
	for (ssize_t i = 0, j = size - 1; i < size / 2; ++i, --j) {
		tmp = buf[i];
		buf[i] = buf[j];
		buf[j] = tmp;
	}
}

std::istream& operator>>(std::istream& input, BigInteger& bi1) {
	bi1.size = 0;
	bi1.sign = 1;
	bi1.body.clear();
	std::string s;
	getline(input, s);
	//std::cout << s << '\n';
	char chunk[10];
	memset(chunk, '0', sizeof(char) * 10);
	size_t length = s.length();
	size_t index = 0;
	for (size_t i = length - 1; i != 0; --i) {
		chunk[index++] = s[i];
		if (index == 9) {
			_reverse(chunk, 9);
			chunk[index] = '\0';
			bi1.body.push_back(atoi(chunk));
			++bi1.size;
			index = 0;
		}
	}
	if (s[0] == '-') {
		bi1.sign = -1;
	}
	else {
		chunk[index++] = s[0];
	}
	if (index != 0) {
		chunk[index] = '\0';
		_reverse(chunk, index);
		bi1.body.push_back(atoi(chunk));
		++bi1.size;
	}
	return input;
}

std::ostream& operator<<(std::ostream& output, const BigInteger& bi1) {
	if (bi1.sign == -1) output << '-';
	ssize_t i = bi1.size - 1;
	for (; i > 0; --i) {
		if (bi1.body[i] != 0) break;
	}
	output << bi1.body[i];
	for (--i; i != -1; --i) {
		for (uint32_t j = 0, deg = bi1.RADIX / 10; j < 9; j++, deg /= static_cast<uint32_t>(10)) {
			output << ((bi1.body[i] / deg) % static_cast<uint32_t>(10));
		}
		//output << bi1.body[i];
	}
	//output << '\n' << bi1.size;
	return output;
}

int main() {
	/*BigInteger p1, p2, n1, n2;
	std::cout << "Enter first positive:\n";
	std::cin >> p1;
	std::cout << "Enter second positive:\n";
	std::cin >> p2;
	std::cout << "Enter first negative:\n";
	std::cin >> n1;
	std::cout << "Enter second negative:\n";
	std::cin >> n2;
	std::cout << "p1 + p2 = " << p1 + p2 << "\n";
	std::cout << "p1 - p2 = " << p1 - p2 << '\n';
	std::cout << "n1 + n2 = " << n1 + n2 << '\n';
	std::cout << "n1 - n2 = " << n1 - n2 << '\n';
	std::cout << "p1 + n1 = " << p1 + n1 << '\n';
	std::cout << "p1 - n1 = " << p1 - n1 << '\n';*/
	/*BigInteger a = 999999999;
	BigInteger b = 1;
	std::cout << a + b << ' ' << a + b << '\n';*/

	/*BigInteger a;
	std::cin >> a;
	--a;
	std::cout << a << ' ';
	++a;
	++a;
	std::cout << a << ' ';
	--a;
	std::cout << a << '\n';*/

	BigInteger a, b;
	std::cin >> a >> b;
	std::cout << a << ' ' << b << '\n';
	std::cout << a * b << '\n';
	
	/*int a1, b1;
	std::cin >> a1 >> b1;
	BigInteger a = a1, b = b1;
	a /= b;
	std::cout << "C++ a / b = " << a1/b1 << '\n';
	std::cout << "BigInteger a /= b : " << a << '\n';*/

}
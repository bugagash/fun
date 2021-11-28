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
		if (copy > 1000000000) {
			body.push_back(t % 1000000000);
			body.push_back(t / 1000000000);
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
		for (uint32_t i = 0; i < size; ++i) {
			if (body[i] < bi1.body[i]) return false;
		}
		return true;
	}

	bool check_zero() const {
		if (size == 1 && body[0] == 0) return true;
		return false;
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
			body[i] = tmp % 1000000000;
			CarryFlag = tmp / 1000000000;
		}
		for (; i < size; ++i) {
			tmp = body[i] + CarryFlag;
			body[i] = tmp % 1000000000;
			CarryFlag = tmp / 1000000000;
		}
		for (; i < bi1.size; ++i) {
			tmp = bi1.body[i] + CarryFlag;
			body.push_back(tmp % 1000000000);
			++size;
			CarryFlag = tmp / 1000000000;
		}
		if (CarryFlag) {
			body.push_back(CarryFlag % 1000000000);
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
			tmp = 1000000000 + (index < copy.size) * copy.body[index % copy.size] - (index < copy_min.size) * copy_min.body[index % copy_min.size] - CarryFlag;
			copy.body[index] = tmp % 1000000000;
			CarryFlag = (tmp >= 1000000000 ? 0 : 1);
		}
		*this = copy;
		if (check_zero()) sign = 1;
		return *this;
	}

	BigInteger& operator*=(const BigInteger& bi1) {

	}

	BigInteger& operator/=(const BigInteger& bi1) {
		
	}

	BigInteger operator+(const BigInteger& bi1) const {
		BigInteger copy = *this;
		copy += bi1;
		return copy;
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
		for (int j = 0, deg = 100000000; j < 9; j++, deg /= 10) {
			output << ((bi1.body[i] / deg) % 10);
		}
		//output << bi1.body[i];
	}
	//output << '\n' << bi1.size;
	return output;
}

int main() {
	BigInteger p1, p2, n1, n2;
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
	std::cout << "p1 - n1 = " << p1 - n1 << '\n';
	/*BigInteger a = 999999999;
	BigInteger b = 1;
	std::cout << a + b << ' ' << a + b << '\n';*/
}
#include <vector>
#include <string>

/*enum class sign : int8_t {
	neg = -1,
	zer = 0,
	pos = 1,

};*/

class BigInteger{
private:

	std::vector<uint32_t> body;
	uint32_t sz;
	int8_t sg;

	static const uint32_t RADIX = 1e9;

public:

	class divide_by_zero : public std::exception {};	//That's because we have already started!
	class null_body : public std::exception {};			//That's too!

	void plus(const BigInteger& bi1) {
		uint32_t ptr = 0;
		uint32_t CF = 0;
		uint32_t cur_val = 0;
		for (; ptr < sz && ptr < bi1.sz; ++ptr) {
			cur_val = body[ptr] + bi1.body[ptr] + CF;
			CF = cur_val / RADIX;
			body[ptr] = cur_val % RADIX;
		}
		for (; ptr < bi1.sz; ++ptr) {
			cur_val = bi1.body[ptr] + CF;
			CF = cur_val / RADIX;
			body.push_back(cur_val % RADIX);
			++sz;
		}
		for (; ptr < sz; ++ptr) {
			cur_val = body[ptr] + CF;
			CF = cur_val / RADIX;
			body[ptr] = cur_val % RADIX;
		}

		if (CF != 0) {
			body.push_back(CF);
			++sz;
		}
	}

	bool modul_bigger(const BigInteger& bi) const {
		if (sz != bi.sz) return (sz > bi.sz);
		bool flag = false;
		for (uint32_t i = sz - 1; i != -1; --i) {
			if (body[i] < bi.body[i]) return false;
			if (body[i] > bi.body[i]) flag = true;
		}
		return flag;
	}

	bool modul_equal(const BigInteger& bi) const {
		if (sz != bi.sz) return false;
		for (uint32_t i = 0; i < sz; ++i) {
			if (body[i] != bi.body[i]) return false;
		}
		return true;
	}

	void minus(const BigInteger& bi) {
		if (modul_equal(bi)) {
			sg = 1;
			*this = 0;
			return;
		}
		uint32_t CF = 0, cur_val = 0;
		if (modul_bigger(bi)) {
			uint32_t i = 0;
			for (; i < sz && i < bi.sz; ++i) {
				cur_val = RADIX + body[i] - bi.body[i] - CF;
				body[i] = cur_val % RADIX;
				CF = (cur_val >= RADIX ? 0 : 1);
			}
			for (; i < sz; ++ i) {
				cur_val = RADIX + body[i] - CF;
				body[i] = cur_val % RADIX;
				CF = (cur_val >= RADIX ? 0 : 1);
			}
		}
		else {
			sg = -sg;
			uint32_t i = 0;
			for (; i < sz && i < bi.sz; ++i) {
				cur_val = RADIX + bi.body[i] - body[i] - CF;
				body[i] = cur_val % RADIX;
				CF = (cur_val >= RADIX ? 0 : 1);
			}
			for (; i < bi.sz; ++i) {
				cur_val = RADIX + bi.body[i] - CF;
				body.push_back(cur_val % RADIX);
				CF = (cur_val >= RADIX ? 0 : 1);
				++sz;
			}
		}
	}

	void make_zero_size(uint32_t size = static_cast<uint32_t>(0)) {
		body.clear();
		sz = 0;
		sg = 1;
		for (uint32_t i = 0; i < size; ++i) {
			body.push_back(static_cast<uint32_t>(0));
			++sz;
		}
	}

	std::string to_String() const {
		if (body.size() == 0) throw BigInteger::null_body();
		std::string str;
		uint32_t i = 0, deg = RADIX / 10;
		if (*this == static_cast<BigInteger>(0)) {
			str.push_back('0');
			return str;
		}
		if (sg == -1) str.push_back('-');

		uint32_t pos = sz - 1;
		for (; pos > 0; --pos) {
			if (body[pos] != 0) break;
		}

		for (i = 0, deg = RADIX / 10; i < 9 && (((body[pos] / deg) % 10) == 0); ++i, deg /= 10);
		for (; i < 9; ++i, deg /= 10) {
			str.push_back('0' + ((body[pos] / deg) % 10));
		}

		for (--pos; pos != -1; --pos) {
			for (i = 0, deg = RADIX / 10; i < 9; i++, deg /= 10) {
				str.push_back('0' + ((body[pos] / deg) % 10));
			}
		}

		return str;
	}

	BigInteger() {
		sz = 0;
		sg = 1;
	}

	BigInteger(const int& n) {
		if (n == 0) {
			sg = 1;
			sz = 1;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}
		int copy = n;
		sg = 1;
		sz = 0;
		if (n < 0) {
			copy = -copy;
			sg = -sg;
		}

		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const uint32_t& n) {
		if (n == 0) {
			sz = 1;
			sg = 1;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}
		uint32_t copy = n;
		sg = 1;
		sz = 0;
		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const ssize_t& n) {
		if (n == 0) {
			sg = 1;
			sz = 1;
			body.push_back(0);
			return;
		}
		size_t copy = n;
		sg = 1;
		sz = 0;
		if (n < 0) {
			copy = -copy;
			sg = -sg;
		}

		while (copy > 0) {
			body.push_back(copy % RADIX);
			copy /= RADIX;
			++sz;
		}
	}

	BigInteger(const BigInteger& bi1) {
		if (this == &bi1) return;

		sg = bi1.sg;
		sz = bi1.sz;

		body.clear();

		for (uint32_t i = 0; i < sz; ++i) body.push_back(bi1.body[i]);
	}

	bool operator==(const BigInteger& bi) const {
		if (sz != bi.sz || sg != bi.sg) {
			return false;
		}
		for (uint32_t i = 0; i < sz; ++i) {
			if (body[i] != bi.body[i]) return false;
		}
		return true;
	}

	bool operator!=(const BigInteger& bi) const {
		return !(*this == bi);
	}

	bool operator<(const BigInteger& bi) const { //Everithing (sometimes except "==") define only "<"
		if (*this == bi) return false;
		if (sg > bi.sg) return false;
		if (sg < bi.sg) return true;
		if (sz < bi.sz) return (sg == 1);
		if (sz > bi.sz) return !(sg == 1);
		if (sg == -1) {
			for (uint32_t i = 0; i < sz; ++i) {
				if (body[i] < bi.body[i]) return false;
			}
			return true;
		}
		else {
			for (uint32_t i = 0; i < sz; ++i) {
				if (body[i] > bi.body[i]) return false;
			}
			return true;
		}
	}

	bool operator>(const BigInteger& bi1) const {
		return !(*this < bi1);
	}

	bool operator>=(const BigInteger& bi1) const {
		return (*this > bi1) || (*this == bi1);
	}

	bool operator<=(const BigInteger& bi1) const {
		return (*this < bi1) || (*this == bi1);
	}

	uint32_t size() const {
		return sz;
	}

	BigInteger& operator+=(const BigInteger& bi) {
		if (sg == bi.sg) {
			plus(bi);
		}
		else minus(bi);
		return *this;
	}

	BigInteger& operator-=(const BigInteger& bi) {
		if (sg != bi.sg) {
			plus(bi);
		}
		else minus(bi);
		return *this;
	}

	BigInteger& operator++() {
		*this += 1;
		return *this;
	}

	BigInteger operator++(int) {
		BigInteger copy = *this;
		*this += 1;
		return copy;
	}

	BigInteger& operator--() {
		*this -= 1;
		return *this;
	}

	BigInteger operator--(int) {
		BigInteger copy = *this;
		*this -= 1;
		return copy;
	}

	void remove_leading_zeros() {
		while (sz > 1 && body.back() == 0) {
			body.pop_back();
			--sz;
		}
	}

	BigInteger& operator*=(const BigInteger& bi) {
		BigInteger copy = *this;
		uint64_t cur_val = 0;
		uint32_t CF = 0;
		 
		this->make_zero_size(sz + bi.sz);
		 
		for (uint32_t i = 0; i < copy.sz; ++i) {
			for (uint32_t j = 0; (j < bi.sz) || CF; ++j) {
				cur_val = static_cast<uint64_t>(body[i + j] + static_cast<uint64_t>(copy.body[i]) * (j < bi.sz ? bi.body[j] : 0) + CF);
				body[i + j] = static_cast<uint32_t>(cur_val % RADIX);
				CF = static_cast<uint32_t>(cur_val / RADIX);
			}
		}

		this->remove_leading_zeros();

		return *this;
	}

	BigInteger& operator*=(const ssize_t& x) {
		if (*this == 0) return *this;
		uint64_t cur_val = 0;
		uint32_t CF = 0;

		for (uint32_t i = 0; i < sz; ++i) {
			cur_val = (static_cast<uint64_t>(body[i]) + CF) * x;
			body[i] = cur_val % RADIX;
			CF = cur_val / RADIX;
		}

		while (CF) {
			body.push_back(CF % RADIX);
			++sz;
			CF /= RADIX;
		}
		return *this;
	}

	BigInteger& two_dividing() {
		if (*this == 0) {
			return *this;
		}
		uint32_t CF = 0, cur_val = 0;

		for (uint32_t i = sz - 1; i !=  -1; --i) {
			cur_val = CF + body[i] / 2;
			CF = (body[i] % 2 == 0 ? 0 : RADIX / 2);
			body[i] = cur_val;
		}

		this->remove_leading_zeros();
		return *this;
	}

	friend BigInteger operator+(const BigInteger& bi1, const BigInteger& bi2);
	friend BigInteger operator*(const BigInteger& bi1, const BigInteger& bi2);
	//friend BigInteger operator-(const BigInteger& bi1, const BigInteger& bi2);

	void shift_right() {
		if (sz == 0) {
			++sz;
			body.push_back(static_cast<uint32_t>(0));
			return;
		}

		body.push_back(body[sz - 1]);
		++sz;
		for (uint32_t i = sz - 2; i > 0; --i) {
			body[i] = body[i - 1];
		}
		body[0] = 0;
	}

	BigInteger& operator/=(const BigInteger& bi) {
		if (bi == 0) throw BigInteger::divide_by_zero();
		if (*this == 0) return *this;
		if (modul_equal(bi)) {
			sg = sg * bi.sg;
			return *this;
		}
		if (!modul_bigger(bi)) {
			*this = 0;
			return *this;
		}

		//BigInteger right_copy = bi;
		BigInteger left = *this;
		BigInteger cur_val;
		this->make_zero_size(sz);

		for (uint32_t i = left.sz - 1; i >= 0; --i) {
			cur_val.shift_right();
			cur_val.body[0] = left.body[i];
			cur_val.remove_leading_zeros();
			
			uint32_t x = 0, l = 0, r = RADIX;
			while(l <= r) {
				uint32_t m = (l + r) >> 1;
				BigInteger tmp = bi;
				tmp *= m;
				if (tmp <= cur_val) {
					x = m;
					l = m + 1;

				}
				else r = m - 1;
			}

			body[i] = x;

			cur_val -= bi * x;
		}

		this->remove_leading_zeros();
		return *this;
	}

	//BigInteger operator+(const BigInteger& bi) const{}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	//BigInteger operator-(const BigInteger& bi) const{}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	//BigInteger operator*(const BigInteger& bi) const{}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	//BigInteger operator/(const BigInteger& bi) const {}     // IT'S BAD BECAUSE IT DOESNT HAVE EQUAL PARTS

	~BigInteger() {}

	friend std::ostream& operator<<(std::ostream& output, const BigInteger& bi);
};

std::ostream& operator<<(std::ostream& output, const BigInteger& bi) {
	output << bi.to_String();
	return output;
}

BigInteger operator+(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy += bi2;
	return copy;
}

BigInteger operator-(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy -= bi2;
	return copy;
}

BigInteger operator*(const BigInteger& bi1, const BigInteger& bi2) {
	BigInteger copy = bi1;
	copy *= bi2;
	return copy;
}
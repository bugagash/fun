#include <cstring>
#include <iostream>
//#include <time.h>

class String {
public:
	String() {
		size = 0;
		capacity = 16;
		str = new char[16];
	}

	String(const char* arr) {
		if (str) {
			delete[] str;
		}
		capacity = 16;
		size = 0;
		for (size_t i = 0; arr[i]; ++i) ++size;
		for (; capacity < size; capacity *= 2);
		str = new char[capacity];
		memcpy(str, arr, size);
	}
	
	String(size_t n, char c) {
		if (str) {
			delete[] str;
		}
		capacity = 16;
		size = n;
		for (; capacity < size; capacity *= 2);
		str = new char[capacity];
		memset(str, n, c);
	}

	String(const char c) {
		if (str) {
			delete[] str;
		}
		capacity = 1;
		size = 1;
		str = new char[1];
		str[0] = c;
	}

	String(const String& s): size(s.size), capacity(s.capacity) {
		if (str) delete[] str;
		str = new char[capacity];
		memcpy(str, s.str, size);
	}

	~String() {
		delete[] str;
	}

	size_t length() const {
		return size;
	}

	size_t mem() const {
		return capacity;
	}
	
	/*void swap_pointers(char* s1, char* s2) {
		char* tmp = s1;
		s1 = s2;
		s2 = tmp;
	}*/

	void push_back(char c) {
		if (size + 1 > capacity) {
			capacity *= 2;
			char *tmp = new char[capacity];
			memcpy(tmp, str, size);
			delete[] str;
			str = tmp;
		}
		str[size] = c;
		++size;
	} //Amortized O(1)
	
	void pop_back() {
		--size;
		if (size < capacity/4) {
			capacity /= 2;
			char *tmp = new char[capacity];
			memcpy(tmp, str, size);
			delete[] str;
			str = tmp;
		}
	} //Amortized O(1)
	
	char& front() const {
		return str[0];
	}
	
	char& back() const {
		return str[size-1];
	}
	
	size_t find(const String& substr) const {
		size_t i = 0, j = 0;
		for (i = 0; i < size; ++i) {
			for (j = 0; j < substr.size && str[i+j] == substr.str[j]; ++j);
			if (j == substr.size) return i; 
		}
		return size;
	}
	
	size_t rfind(const String& substr) const {
		size_t i = 0, j = 0;
		size_t pos = -1;
		bool check = 0;
		for (i = 0; i < size; ++i) {
			for (j = 0; j < substr.size && str[i+j] == substr.str[j]; ++j);
			if (j == substr.size) {
				pos = i;
				check = 1;
			} 
		}
		if (check) return pos;
		return size;
	}
	
	String substr(size_t start, size_t count) const {
		String tmp;
		for (size_t i = 0; i < count && i + start < size; ++i) {
			tmp.push_back(str[i + start]);
		}
		return tmp;
	}
	
	bool empty() {
		return size == 0;
	}
	
	void clear() {
		size = 0;
		capacity = 16;
		delete[] str;
		str = new char [capacity];
	}
	
	bool operator==(const String& s) const {
		size_t i = 0;
		if (size != s.size) return false;
		for (; i < size && str[i] == s.str[i]; ++i);
		if (i != size) return false;
		return true;
	}
	
	char& operator[](const int index) {
		return str[index];
	}
	
	/*String& operator+=(const String& s) {
		size_t new_size = size + s.size;
		for(; new_size > capacity; capacity *= 2);
		char* tmp = new char[capacity];
		memcpy(tmp, str, size);
		memcpy(tmp+size, s.str, s.size);
		delete[] str;
		str = tmp;
		size = new_size;
		return *this;
	}*/
	//Next realization is better:

	String& operator+=(const String& s) {
		for (size_t i = 0; i < s.size; i++) {
			this->push_back(s.str[i]);
		}
		size += s.size;
		return *this;
	}

	String& operator=(const String& s) {
		delete[] str;
		size = s.size;
		capacity = s.capacity;
		str = new char[size];
		memcpy(str, s.str, size);
		return *this;
	}

	//Testing += for quest number:
	//1.69146 -- first for 100000 1e5
	//9.73115 -- second for 100000000 1e8

	//Testing += for quest length:
	//4.1e-05 -- first for 1e5
	//0.01341 -- second for 1e5

	String& operator+=(const char c) {
		this->push_back(c);
		return *this;
	}

	friend std::istream& operator>>(std::istream& input, const String& str);
	friend std::ostream& operator<<(std::ostream& output, const String& str);

private:
char* str = nullptr;
size_t size, capacity;

};

std::istream& operator>>(std::istream& input, String& str) {
	if (str.length() != 0) {
		str.clear();
	}
	char c;
	input.get(c);
	while ((((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) && (c != ' ') && (c != '\n')) {
		str.push_back(c);
		input.get(c);
	}
	return input;
}

std::ostream& operator<<(std::ostream& output, const String& str) {
	for (size_t i = 0; i < str.size; i++) {
		output << str.str[i];
	}
	return output;
}

String operator+(const String& s1, const String& s2) {
	String tmp;
	tmp += s1;
	tmp += s2;
	return tmp;
}
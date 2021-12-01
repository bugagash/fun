#include <cstring>
#include <iostream>
//#include <time.h>

class String {
public:
	String() {
		size = 0;
		capacity = 1;
		str = new char[1];
	}

	String(const char* arr) {
		capacity = 1;
		size = 0;
		for (size_t i = 0; arr[i]; ++i) ++size;
		capacity = 2*size;
		str = new char[capacity];
		memcpy(str, arr, size);
	}
	
	String(size_t n, char c) {
		capacity = 2*n;
		size = n;
		str = new char[capacity];
		memset(str, c, n);
	}

	String(const char c) {
		capacity = 1;
		size = 1;
		str = new char[1];
		str[0] = c;
	}

	String(const String& s): size(s.size), capacity(s.capacity) {
		str = new char[capacity];
		memcpy(str, s.str, size);
	}

	~String() {
		delete [] str;
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
		if (size >= capacity) {
			capacity *= 2;
			char *tmp = new char[capacity];
			memcpy(tmp, str, size);
			delete [] str;
			str = tmp;
		}
		str[size] = c;
		++size;
	} //Amortized O(1)
	
	void pop_back() {
		if (!size) return;
		--size;
		if (size < capacity/4) {
			capacity /= 2;
			char *tmp = new char[capacity];
			memcpy(tmp, str, size);
			delete [] str;
			str = tmp;
		}
	} //Amortized O(1)
	
	char& front() {
		return str[0];
	}

	const char& front() const {
		return str[0];
	}
	
	char& back() {
		return str[size-1];
	}

	const char& back() const {
		return str[size-1];
	}
	
	size_t find(const String& s) const {
		if (size == 0 || s.size == 0 || size < s.size) return size;
		size_t i;
		for (i = 0; i <= size - s.size; ++i) {
			if (substr(i, s.size) == s) return i;
		}
		return size;
	}
	
	size_t rfind(const String& s) const {
		if (size == 0 || s.size == 0 || size < s.size) return size;
		size_t i, pos = size;
		for (i = 0; i <= size - s.size; ++i) {
			if (substr(i, s.size) == s) {
				pos = i;
			} 
		}
		return pos;
	}
	
	String substr(size_t start, size_t count) const {
		String tmp;
		for (size_t i = 0; i < count; ++i) {
			tmp.push_back(str[i + start]);
		}
		return tmp;
	}
	
	bool empty() const {
		return (size == 0);
	}
	
	void clear() {
		size = 0;
		capacity = 1;
		delete [] str;
		str = new char[capacity];
	}
	
	bool operator==(const String& s) const {
		size_t i = 0;
		if (size != s.size) return false;
		for (; i < size; ++i) {
			if (str[i] != s.str[i]) return false;
		}
		return true;
	}
	
	char& operator[](const int index) {
		return str[index];
	}

	const char& operator[](const int index) const {
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
		size_t _size = s.size;
		for (size_t i = 0; i < _size; i++) {
			push_back(s.str[i]);
		}
		return *this;
	}

	String& operator=(const String& s) {
		if (&s == this) return *this;
		delete [] str;
		size = s.size;
		capacity = s.capacity;
		str = new char[capacity];
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
		push_back(c);
		return *this;
	}

	friend std::istream& operator>>(std::istream& input, const String& str);
	friend std::ostream& operator<<(std::ostream& output, const String& str);

private:
char* str = nullptr;
size_t size, capacity;

};

std::istream& operator>>(std::istream& input, String& s) {
	if (s.length() != 0) {
		s.clear();
	}
	char buf[1000];
	input.getline(buf, 1000);
	for (size_t i = 0; buf[i]; ++i) {
		s.push_back(buf[i]);
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
	String tmp = s1;
	tmp += s2;
	return tmp;
}
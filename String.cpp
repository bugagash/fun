#include <cstring>
#include <iostream>

class String {
public:
	String() {
		size = 0;
		capacity = 16;
		str = new char[16];
	}

	String(const char* cstr) {
		capacity = 16;
		size = 0;
		for (size_t i = 0; cstr[i+1]; ++i) ++size;
		for (; capacity < size; capacity *= 2);
		str = new char[capacity];
		memcpy(str, cstr, size);
	}
	
	String(size_t n, char c) {
		capacity = 16;
		size = n;
		for (; capacity < size; capacity *= 2);
		str = new char[capacity];
		memset(str, n, c);
	}

	String(const String& s): size(s.size), capacity(s.capacity) {
		str = new char[capacity];
		memcpy(str, s.str, size);
	}

	~String() {
		delete[] str;
	}

	size_t length() const {
		return size;
	}
	
	/*void swap_pointers(char* s1, char* s2) {
		char* tmp = s1;
		s1 = s2;
		s2 = tmp;
	}*/

	void push_back(char c) {
		++size;
		if (size > capacity) {
			capacity *= 2;
			char *tmp = new char[capacity];
			memcpy(tmp, str, size-1);
			delete[] str;
			str = tmp;
		}
		str[size] = c;
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
		return str[size-1];
	}
	
	char& back() {
		return str[0];
	}
	
	size_t find(const String& substr) {}
	
	size_t rfind(const String& substr) {}
	
	String substr(size_t start, size_t count) {}
	
	bool empty() {
		return size == 0;
	}
	
	void clear() {
		size = 0;
	}
	
	bool operator==(const String& s) const {
		size_t i = 0;
		if (size != s.size) return false;
		for (; i < size && str[i] == s.str[i]; ++i);
		if (i != size-1) return false;
		else return true;
	}
	
	char operator[](const int index) const {
		return str[index];
	}
	
	String& operator+=(const String& str) {}

private:
char* str = nullptr;
size_t size, capacity;

};

std::istream& operator>>(std::istream& input, const String& str) {}

std::ostream& operator<<(std::ostream& output, const String& str) {}

int main() {

}
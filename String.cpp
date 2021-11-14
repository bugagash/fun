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
		if (str) {
			delete[] str;
		}
		capacity = 16;
		size = 0;
		for (size_t i = 0; cstr[i]; ++i) ++size;
		for (; capacity < size; capacity *= 2);
		str = new char[capacity];
		memcpy(str, cstr, size);
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
	
	char& back() {
		return str[size-1];
	}
	
	size_t find(const String& substr) {
		size_t i = 0, j = 0;
		for (i = 0; i < size; ++i) {
			for (j = 0; j < substr.size && str[i+j] == substr.str[j]; ++j);
			if (j == substr.size) return i; 
		}
		return size;
	}
	
	size_t rfind(const String& substr) {
		size_t i = 0, j = 0;
		size_t pos = -1;
		for (i = 0; i < size; ++i) {
			for (j = 0; j < substr.size && str[i+j] == substr.str[j]; ++j);
			if (j == substr.size) pos = i; 
		}
		if (pos != -1) return pos;
		return size;
	}
	
	String substr(size_t start, size_t count) {
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
	
	char operator[](const int index) const {
		return str[index];
	}
	
	String& operator+=(const String& str) {
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
	while (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) && (c != ' ') && (c != '\n')) {
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

int main() {
	String p = "moreradosti";
	String c = p.substr(0,1);
	std::cout << p.front() << ' ' << p.back() << '\n';
	std::cout << c << '\n';
}






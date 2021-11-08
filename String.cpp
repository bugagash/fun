#include <cstring>
#include <iostream>

class String {
public:
	String() = delete; //This isn't constructor
	
	String(size_t n, char c): str(new char[n]), size(n) {
		memset(str, c, n);
	}

	String(const String& s): String(s.size, '\0') {
		memcpy(str, s.str, size);
	}

	void print() {
		for (size_t i = 0; i < size; i++) {
			std::cout << str[i];
		}
		std::cout << '\n';
	}

	void swap(String& s) {
		std::swap(str, s.str);
		std::swap(size, s.size);
	}

	//Copy and swap idiom
	String& operator=(const String& s) {
		String copy = s;
		swap(copy);
		return *this;
		/*delete[] str;
		str = new char[s.size];
		size = s.size;
		memcpy(str, s.str, size);
		return *this;*/
	}

	String& operator=(const char* arr) {
		if (str) {delete[] str; size = 0; }
		for (size_t i = 0; arr[i]; ++i) size++;
		memcpy(str, arr, size);
		return *this;
	}

	String& operator+=(const String& s) {
		String p = *this;
		delete[] this->str;
		str = new char[size + s.size];
		memcpy(str, p.str, size);
		memcpy(str + size, s.str, s.size);
		size += s.size;
		return *this;		
	}

	String& operator+=(const char& c) {
		return *this += String(1, c);
	}

	char& operator[](size_t index) {
		return str[index];
	}

	~String() {
		delete[] str;
	} 


private:
	char *str = nullptr;
	size_t size = 0;
};

String operator+(const String& s1, const String& s2) {
	String copy = s1;
	return copy += s2;
}

String operator+(const String& s1, const char& c) {
	return s1 +  String(1, c);
}

int main() {
	String *q = nullptr;
	String s(10, 'a');
	s.print();
	s += (String (11, '1') + 'b');
	s.print();
	//String p = "aaaaa";
	std::cout << s[2] << '\n';
	{
		String p(11, 'p');
		q = &p;
	}
	//String s(10 ,'a');

	std::cout << sizeof(s) << ' ' << sizeof(*q) << '\n';
}
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

	~String() {
		delete[] str;
	} 

private:
	char *str = nullptr;
	size_t size = 0;
};

int main() {
	String *q = nullptr;
	{
		String p(11, 'p');
		q = &p;
	}
	String s(10 ,'a');
	std::cout << sizeof(s) << ' ' << sizeof(*q) << '\n';
}
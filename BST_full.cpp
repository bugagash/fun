#include <stdio.h>

template<typename T>
class BST {
public:

private:
	struct BSNode {
		T key;
		BSNode *left = nullptr;
		BSNode *right = nullptr;
		BSNode(T val): key(val) {}
		~BSNode() {}

		void insert(T key) {
			BSNode *ptr = nullptr;
			if (key > this->key) ptr = this->right;
			else ptr = this->right;
			if (!ptr) ptr = new BSNode(key);
			else ptr->insert(key);
		}
	};

	BSNode *root = nullptr;
};

int main() {

}
#include <stdio.h>

template<typename T>
class BST {
public:
	BST() {root = nullptr;}
	~BST() {}

	void insert(T val) {
		if (!root) root = new BSNode(val);
		else root->insert(val);
	}

	bool find(T key) {
		BSNode *ptr = root->find(key);
		if (!ptr) return false;
		return true;
	}

	void print() {
		root->print(0);
	}

private:
	struct BSNode {
		T key;
		BSNode *left = nullptr;
		BSNode *right = nullptr;
		BSNode(T val): key(val) {}
		~BSNode() {}

		void insert(T key) {
			BSNode *ptr = key > this->key ? this->right : this->left;
			if (!ptr) ptr = new BSNode(key);
			else ptr->insert(key);
		}

		BSNode *find(T key) {
			if (key == this->key) return this;
			BSNode *ptr = nullptr;
			ptr = key > this->key ? this->right : this->left;
			if (ptr == nullptr) return nullptr;
			else return ptr->find(key);
		}

		void print(int tab) {
			if (!right) right->print(tab + 1);
			for (int i = 0; i < tab; i++) printf(" ");
			printf("%d\n", key);
			if (!left) left->print(tab + 1);
		}
	};

	BSNode *root;
};

int main() {
	BST<int> b;
	b.insert(2);
	b.insert(3);
	b.insert(31);
	b.insert(1);
	printf("%d\n", b.find(1));
	b.print();
}
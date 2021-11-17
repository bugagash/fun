#include <stdio.h>

template<typename T>
class BST {
public:
	BST() {root = nullptr;}
	~BST() {}

	void insert(T _key) {
		if (!root) root = new BSNode(_key);
		else root->insert(_key);
	}

	bool find(T _key) {
		BSNode *ptr = root->find(_key);
		if (ptr == nullptr) return false;
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
		
		~BSNode() {
			if (left) delete[] left;
			if (right) delete[] right;
		}

		BSNode *insert(T _key) {
			//BSNode *ptr = _key > this->key ? right : left;
			//ptr == nullptr ? new BSNode(_key) : ptr->insert(_key);
			if (_key > this->key) right == nullptr ? new BSNode(_key) : right->insert(_key);
			else left == nullptr ? new BSNode(_key) : left->insert(_key);
			return this;
		}

		BSNode *find(T _key) {
			if (_key == this->key) return this;
			BSNode *ptr = _key > this->key ? this->right : this->left;
			if (ptr == nullptr) return nullptr;
			else return ptr->find(_key);
		}

		void print(int tab) const {
			if (right) right->print(tab + 1);
			for (int i = 0; i < tab; i++) printf(" ");
			printf("%d\n", this->key);
			if (left) left->print(tab + 1);
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
	printf("%d\n", b.find(3));
	printf("_______________\n");
	b.print();
}
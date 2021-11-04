#include <stdio.h>

class bst {
public:
  bst() {
    head = nullptr;
  }
  
  ~bst() {}

  void insert(int val) {
    if (head == nullptr) { head = new node(val); return; }
    head->insert(val);
  }

  void print() {
    head->print(0);
  }

  bool find(const int val) {
    node *ptr = head->find(val);
    if (ptr == nullptr) return false;
    return true;
  }

private:
  struct node {
    int val;
    node *left;
    node *right;
    node (int val): val(val) {
      this->left = nullptr;
      this->right = nullptr;
    }

    void print(int tab) {
      if (this->left != nullptr) this->left->print(tab+1);
      for (int i = 0; i < tab; ++i) printf(" ");
      printf("%d\n", this->val);
      if (this->right != nullptr) this->right->print(tab+1);
    }

      node  *insert (int val) {
      node *ptr = nullptr;
      if (val > this->val) right = (right == nullptr ? new node(val) : right->insert(val));
      else left = left == nullptr ? new node(val) : left->insert(val);
      return this;
    }

    node *find(int val) {
      node *ptr = nullptr;
      if (val == this->val) return this;
      if (val > this->val) ptr = this->right;
      else ptr = this->left;
      if (ptr == nullptr) return nullptr;
      return ptr->find(val);
    }
  };

  node *head = nullptr;
};

int main() {
  bst b1;
  b1.insert(12);
  b1.insert(1);
  b1.insert(0);
  b1.insert(2);
  b1.print();
}

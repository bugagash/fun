#include <stdio.h>

class bst {
public:
  bst() {head = nullptr; }
  ~bst() {}

  void insert(int _key) {
    if (head == nullptr) head = new node(_key);
    else head = head->insert(_key);
  }

  void print() {
    head->print(0);
  }

private:
  struct node {
    int key;
    node *left, *right;

    node(int _key): key(_key) {
      left = nullptr;
      right = nullptr;
    }

    node *insert(int _key) {
    node *cur = (_key > key ? right : left);
    if (cur == nullptr) cur = new node(_key);
    else cur = cur->insert(_key);
    return this;
    }

    void print(int shift) {
      if (left != nullptr) left->print(shift+1);
      for (int i = 0; i < shift; ++i) printf(" ");
      printf("%d\n", this->key);
      if (right != nullptr) right->print(shift+1);
    }
  };
  node *head;
};

int main() {
  bst b1;
  b1.insert(5);
  b1.insert(4);
  b1.insert(6);
  b1.insert(3);
  b1.insert(7);
  b1.insert(2);
  b1.insert(1);
  b1.insert(8);
  b1.insert(0);
  b1.insert(9);
  /*for (int i = 0; i < 10; i++) {
    printf("%d -> %d\n", i, b1,find(i));
  }*/
  printf("____________\n");
  b1.print();
}









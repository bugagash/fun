#include <stdio.h>
#include <iostream>
#include <typeinfo>

class bst {
public:
  bst() {head = nullptr; }
  ~bst() {}

  void insert(int _key) {
    if (head == nullptr) head = new node(_key);
    else head->insert(_key);
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

    void insert(int _key) {
      node*& ptr = (_key > key ? right : left);
      std::cout << typeid(ptr).name() << ' ' << typeid(this).name() << '\n';
      if (ptr == nullptr) { ptr = new node(_key); std::cout << ptr << " | " << left << '\n'; }
      else { (ptr)->insert(_key); std::cout << ptr << ' ' << left << '\n'; }
      /*if (_key > key) {
        if (right == nullptr) right = new node(_key);
        else right->insert(_key);
      }
      else {
        if (left == nullptr) left = new node(_key);
        else left->insert(_key);
      }*/
      //return this;
    }

    void print(int shift) {
      if (left != nullptr) left->print(shift+1);
      for (int i = 0; i < shift; ++i) printf("  ");
      printf("%d\n", this->key);
      if (right != nullptr) right->print(shift+1);
    }
  };
  node *head;
};

int main() {
  bst b1;
  b1.insert(14);
  b1.insert(31);
  b1.insert(2);
  b1.insert(1);
  b1.insert(10);
  b1.insert(3);
  /*for (int i = 0; i < 10; i++) {
    printf("%d -> %d\n", i, b1,find(i));
  }*/
  printf("____________\n");
  b1.print();
}









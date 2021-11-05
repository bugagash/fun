#include <stdio.h>

template<typename T>
struct BSNode {
  T val;
  BSNode *left = nullptr;
  BSNode *right = nullptr;

  BSNode (T val) : val(val) {}
  ~BSNode() {}

  void insert(T val) {
    BSNode *ptr = nullptr;
    if (val > this->val) ptr = right;
    else ptr = left;
    if (ptr == nullptr) ptr = new BSNode(val);
    else ptr = ptr->insert(val);
  }
};


template<typename T>
class BSTree {
public:
  BSTree() {}
  ~BSTree() {}

private:
  BSNode *root = nullptr;

};


int main() {

}
#include <stdio.h>
#include <iostream>
#define MAXLEN 1000

class stack {
public:
  stack() { //constructor
    head = new node(0); 
  }

  stack(int val) {
    head = new node(0);
    push(val);
  }

  stack(const stack& st) { //copy_construcor
    head = new node(0);
    for (auto p = st.head->prev, q = head;
         p!= nullptr;
         p = p->prev, q = q->prev) {
      q->prev = new node(p->val);
    }
  }

  ~stack() {
    for (auto p = head->prev; p != nullptr; ) {
      node *tmp = p->prev;
      delete p;
      p = tmp;
    }
  }

  void push(int val) {
    node* tmp = new node(val);
    tmp->prev = head->prev;
    head->prev = tmp;
    printf("ok\n");
  }

  int pop() {
    node* tmp = head->prev;
    head->prev = tmp->prev;
    int v = tmp->val;
    delete tmp;
    return v; 
  }

  int top() {
    return head->prev->val;
  }

  int size() {
    size_t size = 0;
    for (node* p = head->prev; p!= nullptr; p = p->prev) size++;
    return size;
  }

  void print() {
    for (node* p = head->prev; p != nullptr; p = p->prev) {
      printf("%d\n", p->val);
    }
  }

private:
  struct node{
    int val;
    node *prev;
    node (int val): val(val) {
      this->prev = nullptr;
    }
  };
  node* head;
};

bool check_size(char *str1) {
  if (str1[0] != 's' && str1[1] != 'i' && str1[2] != 'z' && str1[3] != 'e') return false;
  return true;
}

bool check_top(char *str) {
  if (str[0] != 't' && str[1] != 'o' && str[3] != 'p') return false;
  return true;
}

bool check_pop(char *str) {
  if (str[0] != 'p' && str[1] != 'o' && str[2] != 'p') return false;
  return true;
}

bool check_push(char *str) {
  if (str[0] != 'p' && str[1] != 'u' && str[2] != 's' && str[3] != 'h') return false;
  return true;
}

bool check_exit(char *str) {
  if (str[0] != 'e' && str[1] != 'x' && str[2] != 'i' && str[3] != 't') return false;
  return true;
}

int main() {
  char* buf = new char[MAXLEN];
  stack s;
  printf("Hello!\nI can show you some triks\nCheck thsi out:\nCommands: push x; pop, top, size\n");
  fgets(buf, MAXLEN, stdin);
  while (!check_exit(buf)) {
    printf("%s\n", buf);
    if (check_pop(buf)) { printf("%d\n", s.pop()); fgets(buf, MAXLEN, stdin); break; }
    if (check_top(buf)) { printf("%d\n", s.top()); fgets(buf, MAXLEN, stdin); break; } 
    if (check_push(buf)) { printf("%d\n", buf[5]); fgets(buf, MAXLEN, stdin); break; }
    if (check_size(buf)) { printf("%d\n", s.size()); fgets(buf, MAXLEN, stdin); break; }
  }
  delete[] buf;
}

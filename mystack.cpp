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

bool check(char* str1, char* str2) {
  for (int i = 0; str1[i] && str2[i]; i++) {
    if (str1[i] != str2[i]) return false;
  }
  return true;
}

int main() {
  char* buf = new char[MAXLEN];
  stack s;
  char pop[4] = {'p', 'o', 'p', '\0'};
  char top[4] = {'t', 'o', 'p', '\0'};
  char size[5] = {'s', 'i', 'z', 'e', '\0'};
  char exit[5] = {'e', 'x', 'i', 't', '\0'};
  char push[5] = {'p', 'u', 's', 'h', '\0'};

  fgets(buf, MAXLEN, stdin);
  while(!check(buf, exit)) {
    if (check(buf, top)) printf("%d\n", s.top());
    else if (check(buf, pop)) printf("%d\n", s.pop());
    else if (check(buf, push)) s.push(static_cast<int>(buf[5] - '0'));
    else if (check(buf, size)) printf("%d\n", s.size());
    fgets(buf, MAXLEN, stdin);
  }
  delete[] buf;
    
}

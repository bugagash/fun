#include <iostream>

#define TT printf("%s: line %d\n", __PRETTY_FUNCTION__, __LINE__)
class stack_ll {
public:
    using T = int;
    stack_ll();  // Конструктор, create
    stack_ll(stack_ll const &st);  // Конструктор копирования
    stack_ll & operator=(stack_ll const &st); // Оператор копирования
    ~stack_ll();
    void push(T val);
    T pop();
    T top();
    bool empty() const;
private:
    struct node {
        T val;
        node *prev;
        node(T val): val(val), prev(nullptr) {}
    };
    node *head;
    void destroy();
    void clone(stack_ll const &st);
};

stack_ll::stack_ll() { // Конструктор, create
    TT;
    head = new node(0);
}

stack_ll::stack_ll(stack_ll const &st) { // Конструктор копирования
    TT;
    clone(st);
}

stack_ll & stack_ll::operator=(stack_ll const &st) { // Оператор копирования
    TT;
    destroy();
    clone(st);
    return *this; 
}

stack_ll::~stack_ll() {   // Деструктор, destroy
    TT;
    destroy();
}

void stack_ll::push(T val) {
    TT;
    node *tmp = new node(val);
    tmp->prev = head->prev;
    head->prev = tmp;
}

stack_ll::T stack_ll::pop() {
    TT;
    node *tmp = head->prev;
    head->prev = tmp->prev;
    T val = tmp->val;
    delete tmp;
    return val;    
}

stack_ll::T stack_ll::top() {
    TT;
    return head->prev->val;
}

bool stack_ll::empty() const {
    TT;
    return head->prev == nullptr;
}

void stack_ll::destroy() {
    while (!empty()) {
        pop();
    }
    delete head;
    head = nullptr;
}

void stack_ll::clone(stack_ll const &st) {
    head = new node(0);
    for (auto p = st.head->prev, q = head; 
        p != nullptr; 
        p = p->prev, q = q->prev) {
        q->prev = new node(p->val);
    }
}
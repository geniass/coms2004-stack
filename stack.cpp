#include <iostream>
template <typename T>
class stack {

public:

  // constructor
  stack ();
  // destructor
  ~stack();
  // copy constructor
  stack (const stack&);
  // copy assignment
  stack& operator= (const stack&);
  // move constructor
  stack (stack&&);
  // move assignment
  stack& operator= (stack&&);

  const T& top() const;  // return the top element
  void pop ();  // remove the top element
  void push(const T&); // add element on top of stack
  void push (T&&); // add element on top of stack
  bool empty() const; // is the stack empty?
  void clear(); // remove all elements

private:

  class Node
  {
      public:
          Node (const T& t = T{}, Node* nxt = nullptr) : value{t}, next{nxt} {}
          Node (T&& t, Node* nxt = nullptr) : value{t}, next{nxt} {}

      private:
          T value;
          Node* next;

    friend class stack;
  };

  Node* head;
  Node* tail;

  void init();
  void copy(const stack& st);

};

template <class T>
stack<T>::stack()
{
    init();
}

template <class T>
stack<T>::~stack()
{
    clear();
    delete head;
    delete tail;
}

template <class T>
void stack<T>::copy(const stack& st)
{
    stack<T> s1;
    Node* n = st.head->next;
    while (n != st.tail) {
        s1.push(n->value);
        n = n->next;
    }

    while (!s1.empty()) {
        push(s1.top());
        s1.pop();
    }
}

// copy constructor
template <class T>
stack<T>::stack(const stack& st)
{
    init();
    copy(st);
}

// copy assignment
template <class T>
stack<T>& stack<T>::operator= (const stack& st)
{
    clear();
    copy(st);
    return *this;
}

// move constructor
template <class T>
stack<T>::stack(stack&& st)
{
    head = st.head;
    st.head = nullptr;
}

// move assignment
template <class T>
stack<T>& stack<T>::operator= (stack&& st)
{
    clear();
    head = st.head;
    st.head = nullptr;
}

template <class T>
const T& stack<T>::top() const
{
    if (head->next != tail) {
        return head->next->value;
    } else {
        std::cout <<"problem" << std::endl;
        return head->next->value;    // TODO: use exceptions
    }
}

template <class T>
void stack<T>::pop()
{
    Node* n = head->next;

    if (n != tail) {
        head->next = n->next;

        delete n;
    } else {

        std::cout <<" cant pop problem" << std::endl;
        //TODO: exceptions
    }
}

template <class T>
void stack<T>::push(const T& val)
{
    Node* n = new Node{val, head->next};
    head->next = n;
}

template <class T>
void stack<T>::push(T&& val)
{
    Node* n = new Node{val, head->next};
    head->next = n;
}

template <class T>
bool stack<T>::empty() const
{
    return (head->next == tail);
}

template <class T>
void stack<T>::clear()
{
    while (!empty()) {
        pop();
    }
}

template <class T>
void stack<T>::init()
{
    head = new Node;
    tail = new Node;
    head->next = tail;
}

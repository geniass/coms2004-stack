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


};


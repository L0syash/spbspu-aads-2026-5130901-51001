#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <string>

namespace losev {

template<class T>
struct Node {
  T data;
  Node* next;
  Node(const T& value, Node* n = nullptr);
};

template<class T>
class LIter {
  friend class List<T>;
private:
  Node<T>* ptr_;
  explicit LIter(Node<T>* ptr = nullptr);
public:
  bool operator==(const LIter& other) const;
  bool operator!=(const LIter& other) const;
  T& operator*();
  const T& operator*() const;
  LIter& operator++();
  LIter operator++(int);
};

template<class T>
class List {
public:
  using iterator = LIter<T>;
  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(nullptr); }

  List() : head_(nullptr) {}

  bool empty() const { return head_ == nullptr; }

  void push_front(const T& value) {
    head_ = new Node<T>(value, head_);
  }

  void pop_front() {
    if (head_ == nullptr) return;
    Node<T>* temp = head_;
    head_ = head_->next;
    delete temp;
  }

private:
  Node<T>* head_;
};



}

#endif

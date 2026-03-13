#include "list.h"

namespace losev {

template<class T>
Node<T>::Node(const T& value, Node* n) : data(value), next(n) {}

template struct Node<int>;
template class List<int>;

template<class T>
LIter<T>::LIter(Node<T>* ptr) : ptr_(ptr) {}

template<class T>
bool LIter<T>::operator==(const LIter& other) const {
  return ptr_ == other.ptr_;
}

template<class T>
bool LIter<T>::operator!=(const LIter& other) const {
  return ptr_ != other.ptr_;
}

template class losev::LIter<int>;
}

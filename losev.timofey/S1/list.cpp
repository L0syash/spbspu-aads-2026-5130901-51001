#include "list.h"

namespace losev {

template<class T>
Node<T>::Node(const T& value, Node* n) : data(value), next(n) {}

template struct Node<int>;
template class List<int>;

}
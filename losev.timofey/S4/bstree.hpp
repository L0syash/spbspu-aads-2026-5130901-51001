#ifndef LOSEV_BSTREE_HPP
#define LOSEV_BSTREE_HPP

#include <cstddef>

namespace losev {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class BSTree
{
public:
  BSTree()
    : root_(nullptr)
    , size_(0)
  {}

  bool empty() const
  {
    return size_ == 0;
  }

  size_t size() const
  {
    return size_;
  }

private:
  struct Node
  {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* parent;

    Node(const Key& k, const Value& v, Node* p = nullptr)
      : key(k)
      , value(v)
      , left(nullptr)
      , right(nullptr)
      , parent(p)
    {}
  };

  Node* root_;
  size_t size_;
  Compare comp_;
};

}

#endif

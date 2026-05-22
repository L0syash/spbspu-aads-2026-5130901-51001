#ifndef LOSEV_BSTREE_HPP
#define LOSEV_BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <functional>

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

  void push(const Key& k, const Value& v)
  {
    if (root_ == nullptr)
    {
      root_ = new Node(k, v);
      ++size_;
      return;
    }

    Node* current = root_;
    while (true)
    {
      if (comp_(k, current->key))
      {
        if (current->left == nullptr)
        {
          current->left = new Node(k, v, current);
          ++size_;
          return;
        }
        current = current->left;
      }
      else if (comp_(current->key, k))
      {
        if (current->right == nullptr)
        {
          current->right = new Node(k, v, current);
          ++size_;
          return;
        }
        current = current->right;
      }
      else
      {
        current->value = v;
        return;
      }
    }
  }

  const Value& get(const Key& k) const
  {
    Node* current = root_;
    while (current != nullptr)
    {
      if (comp_(k, current->key))
      {
        current = current->left;
      }
      else if (comp_(current->key, k))
      {
        current = current->right;
      }
      else
      {
        return current->value;
      }
    }
    throw std::out_of_range("key not found");
  }

  Value& get(const Key& k)
  {
    Node* current = root_;
    while (current != nullptr)
    {
      if (comp_(k, current->key))
      {
        current = current->left;
      }
      else if (comp_(current->key, k))
      {
        current = current->right;
      }
      else
      {
        return current->value;
      }
    }
    throw std::out_of_range("key not found");
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

#ifndef LOSEV_BSTREE_HPP
#define LOSEV_BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>
#include <algorithm>

namespace losev {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class BSTree
{
private:
  struct Node
  {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* parent;

    Node(const Key& k, const Value& v, Node* p = nullptr)
      : key(k), value(v), left(nullptr), right(nullptr), parent(p)
    {}
  };

  Node* root_;
  size_t size_;
  Compare comp_;

  Node* findMin(Node* node) const
  {
    while (node->left) node = node->left;
    return node;
  }

  Node* findNode(const Key& k) const
  {
    Node* current = root_;
    while (current)
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
        return current;
      }
    }
    return nullptr;
  }

  void transplant(Node* u, Node* v)
  {
    if (u->parent == nullptr)
    {
      root_ = v;
    }
    else if (u == u->parent->left)
    {
      u->parent->left = v;
    }
    else
    {
      u->parent->right = v;
    }
    if (v != nullptr)
    {
      v->parent = u->parent;
    }
  }

  size_t heightRecursive(Node* node) const
  {
    if (node == nullptr)
    {
      return 0;
    }
    return 1 + std::max(heightRecursive(node->left), heightRecursive(node->right));
  }

public:
  class Iterator
  {
  private:
    Node* ptr_;

    void goToNext()
    {
      if (ptr_->right)
      {
        ptr_ = ptr_->right;
        while (ptr_->left) ptr_ = ptr_->left;
      }
      else
      {
        Node* parent = ptr_->parent;
        while (parent && ptr_ == parent->right)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

    void goToPrev()
    {
      if (ptr_->left)
      {
        ptr_ = ptr_->left;
        while (ptr_->right) ptr_ = ptr_->right;
      }
      else
      {
        Node* parent = ptr_->parent;
        while (parent && ptr_ == parent->left)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

  public:
    explicit Iterator(Node* ptr = nullptr) : ptr_(ptr) {}

    bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }

    std::pair<const Key, Value&> operator*()
    {
      return std::pair<const Key, Value&>(ptr_->key, ptr_->value);
    }

    Iterator& operator++()
    {
      goToNext();
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp = *this;
      goToNext();
      return tmp;
    }

    Iterator& operator--()
    {
      goToPrev();
      return *this;
    }

    Iterator operator--(int)
    {
      Iterator tmp = *this;
      goToPrev();
      return tmp;
    }
  };

  class ConstIterator
  {
  private:
    const Node* ptr_;

    void goToNext()
    {
      if (ptr_->right)
      {
        ptr_ = ptr_->right;
        while (ptr_->left) ptr_ = ptr_->left;
      }
      else
      {
        const Node* parent = ptr_->parent;
        while (parent && ptr_ == parent->right)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

    void goToPrev()
    {
      if (ptr_->left)
      {
        ptr_ = ptr_->left;
        while (ptr_->right) ptr_ = ptr_->right;
      }
      else
      {
        const Node* parent = ptr_->parent;
        while (parent && ptr_ == parent->left)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

  public:
    explicit ConstIterator(const Node* ptr = nullptr) : ptr_(ptr) {}

    ConstIterator(const Iterator& other) : ptr_(other.ptr_) {}

    bool operator==(const ConstIterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const ConstIterator& other) const { return ptr_ != other.ptr_; }

    std::pair<const Key, const Value&> operator*() const
    {
      return std::pair<const Key, const Value&>(ptr_->key, ptr_->value);
    }

    ConstIterator& operator++()
    {
      goToNext();
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator tmp = *this;
      goToNext();
      return tmp;
    }

    ConstIterator& operator--()
    {
      goToPrev();
      return *this;
    }

    ConstIterator operator--(int)
    {
      ConstIterator tmp = *this;
      goToPrev();
      return tmp;
    }
  };

  using iterator = Iterator;
  using const_iterator = ConstIterator;

  BSTree() : root_(nullptr), size_(0) {}

  ~BSTree() { clear(); }

  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }

  void push(const Key& k, const Value& v)
  {
    if (!root_)
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
        if (!current->left)
        {
          current->left = new Node(k, v, current);
          ++size_;
          return;
        }
        current = current->left;
      }
      else if (comp_(current->key, k))
      {
        if (!current->right)
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
    while (current)
    {
      if (comp_(k, current->key)) current = current->left;
      else if (comp_(current->key, k)) current = current->right;
      else return current->value;
    }
    throw std::out_of_range("key not found");
  }

  Value& get(const Key& k)
  {
    Node* current = root_;
    while (current)
    {
      if (comp_(k, current->key)) current = current->left;
      else if (comp_(current->key, k)) current = current->right;
      else return current->value;
    }
    throw std::out_of_range("key not found");
  }

  Value drop(const Key& k)
  {
    Node* target = findNode(k);
    if (!target)
    {
      throw std::out_of_range("key not found");
    }

    Value result = std::move(target->value);

    if (target->left == nullptr)
    {
      transplant(target, target->right);
    }
    else if (target->right == nullptr)
    {
      transplant(target, target->left);
    }
    else
    {
      Node* successor = findMin(target->right);
      if (successor->parent != target)
      {
        transplant(successor, successor->right);
        successor->right = target->right;
        successor->right->parent = successor;
      }
      transplant(target, successor);
      successor->left = target->left;
      successor->left->parent = successor;
    }

    delete target;
    --size_;
    return result;
  }

  size_t height() const
  {
    return heightRecursive(root_);
  }

  const_iterator rotateLeft(const_iterator it)
  {
    Node* x = const_cast<Node*>(it.ptr_);
    if (x == nullptr || x->right == nullptr)
    {
      return it;
    }

    Node* y = x->right;
    Node* parent = x->parent;

    x->right = y->left;
    if (y->left != nullptr)
    {
      y->left->parent = x;
    }

    y->left = x;
    x->parent = y;
    y->parent = parent;

    if (parent == nullptr)
    {
      root_ = y;
    }
    else if (parent->left == x)
    {
      parent->left = y;
    }
    else
    {
      parent->right = y;
    }

    return const_iterator(y);
  }

  const_iterator rotateRight(const_iterator it)
  {
    Node* y = const_cast<Node*>(it.ptr_);
    if (y == nullptr || y->left == nullptr)
    {
      return it;
    }

    Node* x = y->left;
    Node* parent = y->parent;

    y->left = x->right;
    if (x->right != nullptr)
    {
      x->right->parent = y;
    }

    x->right = y;
    y->parent = x;
    x->parent = parent;

    if (parent == nullptr)
    {
      root_ = x;
    }
    else if (parent->left == y)
    {
      parent->left = x;
    }
    else
    {
      parent->right = x;
    }

    return const_iterator(x);
  }

  const_iterator rotateLargeLeft(const_iterator it)
  {
    Node* x = const_cast<Node*>(it.ptr_);
    if (x == nullptr || x->right == nullptr)
    {
      return it;
    }

    Node* y = x->right;
    if (y->left == nullptr)
    {
      return rotateLeft(it);
    }

    Node* z = y->left;
    Node* parent = x->parent;

    x->right = z->left;
    if (z->left != nullptr)
    {
      z->left->parent = x;
    }

    y->left = z->right;
    if (z->right != nullptr)
    {
      z->right->parent = y;
    }

    z->left = x;
    x->parent = z;
    z->right = y;
    y->parent = z;
    z->parent = parent;

    if (parent == nullptr)
    {
      root_ = z;
    }
    else if (parent->left == x)
    {
      parent->left = z;
    }
    else
    {
      parent->right = z;
    }

    return const_iterator(z);
  }

  const_iterator rotateLargeRight(const_iterator it)
  {
    Node* y = const_cast<Node*>(it.ptr_);
    if (y == nullptr || y->left == nullptr)
    {
      return it;
    }

    Node* x = y->left;
    if (x->right == nullptr)
    {
      return rotateRight(it);
    }

    Node* z = x->right;
    Node* parent = y->parent;

    y->left = z->right;
    if (z->right != nullptr)
    {
      z->right->parent = y;
    }

    x->right = z->left;
    if (z->left != nullptr)
    {
      z->left->parent = x;
    }

    z->right = y;
    y->parent = z;
    z->left = x;
    x->parent = z;
    z->parent = parent;

    if (parent == nullptr)
    {
      root_ = z;
    }
    else if (parent->left == y)
    {
      parent->left = z;
    }
    else
    {
      parent->right = z;
    }

    return const_iterator(z);
  }

  iterator begin()
  {
    return root_ ? iterator(findMin(root_)) : iterator(nullptr);
  }

  iterator end() { return iterator(nullptr); }

  const_iterator begin() const
  {
    return root_ ? const_iterator(findMin(root_)) : const_iterator(nullptr);
  }

  const_iterator end() const { return const_iterator(nullptr); }

  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  void clear()
  {
    while (!empty())
    {
      drop(begin()->first);
    }
  }
};

}

#endif

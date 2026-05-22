#ifndef LOSEV_BSTREE_HPP
#define LOSEV_BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>

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

private:
  Node* findMin(Node* node) const
  {
    while (node->left) node = node->left;
    return node;
  }
};

}

#endif

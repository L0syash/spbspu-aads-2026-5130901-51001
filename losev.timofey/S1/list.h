#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <string>

namespace losev {

template<class T>
class List {
public:
  List() {}
  bool empty() const { return true; }
};

}

#endif
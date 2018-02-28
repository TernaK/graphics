#pragma once
#include <array>


namespace graphics {

  template<typename T>
  class Octree {
  private:
    int level;
    T data;
    std::array<std::shared_ptr<Octree<T>>, 8> children;

  public:

    Octree(int _level) : level(_level - 1) {
      if(level != 0) {
        for(auto &child: children) {
          child = std::make_shared<Octree<T>>(level);
        }
      }
    }

    std::shared_ptr<Octree<T>> operator[](int at_index) {
      return children[at_index];
    }

    T& operator()() {
      return data;
    }

    int get_level() {
      return level;
    }

    bool is_leaf() {
      return level == 0;
    }

  };
};

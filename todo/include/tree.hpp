#pragma once

#include "todo.hpp"
#include <unordered_map>
#include <list>

struct Todo;

struct TreeNode {
  std::list<TreeNode *> children;
  Todo *value;
};

struct TodoTree {
  TreeNode *mRoot;
  std::unordered_map<unsigned int, TreeNode *> mMap; // id -> node
  std::list<TreeNode *> mList; // id -> node

  TodoTree();
  ~TodoTree();

  void clear();
  void print();
  void insert(Todo *value);
};

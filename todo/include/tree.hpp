#pragma once

#include "todo.hpp"
#include <functional>
#include <unordered_map>
#include <list>

struct Todo;

struct TreeNode
{
  std::list<TreeNode *> children;
  Todo *value;
  int row = 0, col = 0; // they actually start from 1
};

struct TodoTree
{
  TreeNode *mRoot;
  std::unordered_map<unsigned int, TreeNode *> mMap; // id -> node
  std::list<TreeNode *> mList;                       // id -> node

  TodoTree();
  ~TodoTree();

  void clear();
  void print();
  void insert(Todo *value);
  void traversal(std::function<void(Todo *todo, int row, int col)>);
};

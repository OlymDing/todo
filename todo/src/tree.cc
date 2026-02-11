#include "tree.hpp"
#include <stack>
#include <string>

TodoTree::TodoTree()
{
  mRoot = new TreeNode;
  mRoot->col = 0;
  mRoot->row = 0;
  mRoot->section_title = "";
  mMap[0] = mRoot;
}

TodoTree::~TodoTree() { clear(); }

void TodoTree::clear()
{
  for (auto node : mList)
    delete node;
}

// DFS
void TodoTree::print()
{
  std::stack<TreeNode *> stack;
  stack.push(mRoot);

  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (auto it = node->children.rbegin(); it != node->children.rend(); ++it)
      stack.push(*it);

    if (node != mRoot)
    {
      std::cout << node->section_title << ' ';
      node->value->print();
    }
  }
}

void TodoTree::insert(Todo *value)
{
  if (mMap.find(value->mId) != mMap.end())
  {
    return;
  }

  auto node = new TreeNode;
  node->value = value;
  mMap[value->mId] = node;

  if (mMap.find(value->mParentId) != mMap.end())
  {
    mMap[value->mParentId]->children.push_back(node);
    if (value->mParentId == 0)
      node->section_title = std::to_string(mRoot->children.size());
    else
      node->section_title =
          mMap[value->mParentId]->section_title + '.' +
          std::to_string(mMap[value->mParentId]->children.size());
  }
  mList.push_back(node);
}

void TodoTree::traversal(
    std::function<void(Todo *todo, int row, int col)> callback
)
{
  std::stack<TreeNode *> stack;
  stack.push(mRoot);

  int current_row = 0;
  int prev_col = 1;
  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    for (auto it = node->children.rbegin(); it != node->children.rend(); ++it)
    {
      auto child = *it;
      child->col = node->col + 1;
      stack.push(child);
    }

    if (node != mRoot)
    {
      if (node->col <= prev_col)
        current_row++;

      node->row = current_row;
      callback(node->value, node->row, node->col);

      prev_col = node->col;
    }
  }
}

std::string TodoTree::summary() { return ""; }

#include "tree.hpp"
#include <stack>

TodoTree::TodoTree()
{
  mRoot = new TreeNode;
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

    for (auto child : node->children)
      stack.push(child);

    if (node != mRoot)
    {
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
  }
  mList.push_back(node);
}

void TodoTree::traversal(
    std::function<void(Todo *todo, int row, int col)> callback
)
{
  std::stack<TreeNode *> stack;
  stack.push(mRoot);

  int row_count = 1;
  while (!stack.empty())
  {
    auto node = stack.top();
    stack.pop();

    int count = 0;
    for (auto child : node->children)
    {
      child->col = node->col + 1;
      stack.push(child);
      count++;
    }

    if (node != mRoot)
    {
      callback(node->value, node->row, node->col);
    }
  }
}

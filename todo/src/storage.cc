#include "storage.hpp"
#include <sqlite3.h>
#include <stack>
#include <fmt/format.h>

using TS = TodoStorage;

TS::TodoStorage() {
  // setup sqlite 3
  if (sqlite3_open(DB_NAME, &db) != SQLITE_OK) {
    exit(1);
  }

  const char *createTableSQL = "CREATE TABLE IF NOT EXISTS todos ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "name TEXT NOT NULL,"
                               "timestamp INTEGER NOT NULL,"
                               "duetime INTEGER NOT NULL,"
                               "status INTEGER NOT NULL,"
                               "parent_id INTEGER NOT NULL)";
  char *errMsg = nullptr;
  if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) !=
      SQLITE_OK) {
    sqlite3_free(errMsg);
    exit(1);
  }
}

TS::~TodoStorage() { sqlite3_close(db); }

bool TS::insert(const std::string &name, int parentId,
                unsigned long long dueTime) {
  Todo item(name, parentId, dueTime);
  const char *insertSQL = "INSERT INTO todos (name, timestamp, status, "
                          "duetime, parent_id) VALUES (?, ?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  sqlite3_bind_text(stmt, 1, item.mName.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 2, item.mTimeStamp);
  sqlite3_bind_int64(stmt, 3, item.mStatus);
  sqlite3_bind_int64(stmt, 4, item.mDueTime);
  sqlite3_bind_int64(stmt, 5, item.mParentId);

  bool success = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);

  return success;
}

TodoTree TS::queryAll() {
  TodoTree tt;
  const char *selectSQL =
      "WITH RECURSIVE todo_tree AS ("
      "    SELECT id, name, timestamp, status, duetime, parent_id"
      "    FROM todos"
      "    WHERE parent_id = 0"
      "    UNION ALL"
      "    SELECT sub_task.id, sub_task.name, sub_task.timestamp, "
      "sub_task.status, "
      "sub_task.duetime, sub_task.parent_id"
      "    FROM todos sub_task"
      "    INNER JOIN todo_tree parent_task ON sub_task.parent_id = "
      "parent_task.id"
      ")"
      "SELECT * FROM todo_tree;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return tt;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    auto item = new Todo;
    item->mId = sqlite3_column_int(stmt, 0);
    item->mName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    item->mTimeStamp = sqlite3_column_int64(stmt, 2);
    item->mStatus = (Todo::Status)sqlite3_column_int(stmt, 3);
    item->mDueTime = sqlite3_column_int(stmt, 4);
    item->mParentId = sqlite3_column_int(stmt, 5);
    // todos.push_back(item);
    tt.insert(item);
  }

  sqlite3_finalize(stmt);
  return tt;
}

Todo TS::query(int id) {
  Todo todo;
  const char *selectSQL =
      "SELECT id, name, timestamp, status, duetime FROM todos WHERE id = ?;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return todo;
  }
  sqlite3_bind_int64(stmt, 1, id);

  int rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    todo.mId = sqlite3_column_int(stmt, 0);
    todo.mName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    todo.mTimeStamp = sqlite3_column_int64(stmt, 2);
    todo.mStatus = (Todo::Status)sqlite3_column_int64(stmt, 3);
    todo.mDueTime = (Todo::Status)sqlite3_column_int64(stmt, 4);
    todo.mIsValid = true;
  }

  sqlite3_finalize(stmt);
  return todo;
}

bool TS::update(const Todo &todo) {
  const char *updateSQL = "UPDATE todos SET status = ?, name = ? WHERE id = ?;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  sqlite3_bind_int(stmt, 1, todo.mStatus);
  sqlite3_bind_text(stmt, 2, todo.mName.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, todo.mId);

  bool success = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);

  return success;
}

bool TS::remove(int id) {
  const char *deleteSQL = "DELETE FROM todos WHERE id = ?;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, deleteSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  sqlite3_bind_int(stmt, 1, id);

  bool success = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);

  return success;
}

bool TS::verifyID(unsigned int id) {
  const char *selectSQL = "SELECT * FROM todos where id = ?;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  sqlite3_bind_int64(stmt, 1, id);

  int rc = sqlite3_step(stmt);
  return rc == SQLITE_ROW;
}

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

std::string TodoTree::summary()
{
  std::string result = "";
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
      auto todo = node->value;
      std::string status_str;
      switch (todo->mStatus)
      {
      case Todo::underway:
        status_str = "underway";
        break;
      case Todo::closed:
        status_str = "closed";
        break;
      case Todo::suspend:
        status_str = "suspend";
        break;
      }
      std::string node_summary = fmt::format(
          "{} Todo Title: {}, Start Date: {}, Status: {}; ",
          node->section_title, todo->mName, timeStamp2date(todo->mTimeStamp),
          status_str
      );
      result += node_summary;
    }
  }
  return result;
}

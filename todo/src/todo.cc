#include "todo.hpp"
#include <sqlite3.h>

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
                               "status INTEGER NOT NULL);";
  char *errMsg = nullptr;
  if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) !=
      SQLITE_OK) {
    sqlite3_free(errMsg);
    exit(1);
  }
}

TS::~TodoStorage() { sqlite3_close(db); }

bool TS::insert(std::string name) {
  Todo item(name);
  const char *insertSQL =
      "INSERT INTO todos (name, timestamp, status) VALUES (?, ?, ?);";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  sqlite3_bind_text(stmt, 1, item.name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 2, item.timestamp);
  sqlite3_bind_int(stmt, 3, item.status);

  bool success = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);

  return success;
}

std::vector<Todo> TS::query() {
  std::vector<Todo> todos;
  const char *selectSQL = "SELECT id, name, timestamp, status FROM todos;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return todos;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Todo item;
    item.id = sqlite3_column_int(stmt, 0);
    item.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    item.timestamp = sqlite3_column_int64(stmt, 2);
    item.status = (Todo::Status)sqlite3_column_int(stmt, 3);
    todos.push_back(item);
  }

  sqlite3_finalize(stmt);
  return todos;
}

bool TS::update(Todo todo) {
  const char *updateSQL = "UPDATE todos SET status = ? WHERE id = ?;";

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  sqlite3_bind_int(stmt, 1, todo.status);
  sqlite3_bind_int(stmt, 2, todo.id);

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

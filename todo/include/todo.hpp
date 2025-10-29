#pragma once
#include <ctime>
#include <string>
#include <vector>

#include <sqlite3.h>

#define DB_NAME "test.db"

struct Todo {

  enum Status {
    ready,
    suspend,
    closed,
  };
  unsigned int id;
  unsigned long long timestamp;
  std::string name;
  Status status;

  Todo() {}
  Todo(std::string name)
      : name(name), timestamp(std::time(0)), status(Status::ready) {}
};

class TodoStorage {
public:
  TodoStorage();  // setup
  ~TodoStorage(); // unsetup

  bool insert(std::string name);
  bool update(Todo todo);
  bool remove(int id);
  std::vector<Todo> query();

private:
  sqlite3 *db;
};

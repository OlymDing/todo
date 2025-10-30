#pragma once
#include "util.hpp"
#include <cstdio>
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
  unsigned long long timeStamp;
  unsigned long long dueTime;
  std::string name;
  Status status;

  Todo() {}
  Todo(std::string name, unsigned long long dueTime = 0)
      : name(name), timeStamp(std::time(0)), status(Status::ready),
        dueTime(dueTime) {}

  void print() {
    printf("%d | %s | %s | %s\n", id, timeStamp2date(timeStamp).c_str(),
           timeStamp2date(dueTime).c_str(), name.c_str());
  }
};

class TodoStorage {
public:
  TodoStorage();  // setup
  ~TodoStorage(); // unsetup

  bool insert(std::string, unsigned long long);
  bool update(Todo);
  bool remove(int);
  std::vector<Todo> queryAll();
  Todo query(int);

private:
  sqlite3 *db;
};

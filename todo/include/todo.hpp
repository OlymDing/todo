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
    underway,
    suspend,
    closed,
  };
  unsigned int mId = 0;
  unsigned long long mTimeStamp;
  unsigned long long mDueTime;
  unsigned int mParentId;
  std::string mName;
  Status mStatus;

  bool isValid = false;

  Todo() {}
  Todo(std::string name, unsigned long long dueTime = 0)
      : mName(name), mTimeStamp(std::time(0)), mStatus(Status::underway),
        mDueTime(dueTime) {}

  void print() {
    std::string statusStr;
    switch (mStatus) {
    case underway:
      statusStr = "underway";
      break;
    case closed:
      statusStr = "closed";
      break;
    case suspend:
      statusStr = "suspend";
      break;
    }
    printf("%d | %s | %s | %s | %s\n", mId, timeStamp2date(mTimeStamp).c_str(),
           timeStamp2date(mDueTime).c_str(), mName.c_str(), statusStr.c_str());
  }

  static void printHeaders() {
    printf("id | create_time | due_time | title | status\n");
  }
};

class TodoStorage {
public:
  TodoStorage();  // setup
  ~TodoStorage(); // unsetup

  bool insert(const std::string &, unsigned int, unsigned long long);
  bool update(const Todo &);
  bool remove(int);
  std::vector<Todo> queryAll();
  Todo query(int);
  bool verifyID(unsigned int id);

private:
  sqlite3 *db;
};

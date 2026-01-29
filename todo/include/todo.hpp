#pragma once
#include "util.hpp"
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <iostream>
#include "tree.hpp"

#define DB_NAME "test.db"

struct TodoTree;

struct Todo
{
  enum Status
  {
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
  bool mIsValid = false;

  Todo() {}

  Todo(std::string name, int parentId, unsigned long long dueTime)
      : mName(name), mTimeStamp(std::time(0)), mStatus(Status::underway),
        mParentId(parentId), mDueTime(dueTime)
  {
  }

  // Todo(Todo &&todo)
  //     : mName(std::move(todo.mName)), mId(todo.mId),
  //       mTimeStamp(todo.mTimeStamp), mDueTime(todo.mDueTime),
  //       mParentId(todo.mParentId), mStatus(todo.mStatus),
  //       mIsValid(todo.mIsValid) {
  //   todo.reset();
  // }

  void print()
  {
    std::string statusStr;
    std::string color_str;
    auto reset_str = "\033[0m";
    switch (mStatus)
    {
    case underway:
      statusStr = "underway";
      color_str = "\033[31m";
      break;
    case closed:
      statusStr = "closed";
      color_str = "\033[32m";
      break;
    case suspend:
      statusStr = "suspend";
      color_str = "\033[33m";
      break;
    }
    std::cout << color_str;
    printf(
        "%d | %s | %s | %s | %s | %d\n", mId,
        timeStamp2date(mTimeStamp).c_str(), timeStamp2date(mDueTime).c_str(),
        mName.c_str(), statusStr.c_str(), mParentId
    );
    std::cout << reset_str;
  }

  void reset()
  {
    mId = 0;
    mTimeStamp = 0;
    mDueTime = 0;
    mParentId = 0;
    mName = "";
    mStatus = (Status)0;
    mIsValid = false;
  }

  static void printHeaders()
  {
    printf("id | create_time | due_time | title | status | parentId\n");
  }
};

class TodoStorage
{
public:
  TodoStorage();  // setup
  ~TodoStorage(); // unsetup

  bool insert(const std::string &, int, unsigned long long);
  bool update(const Todo &);
  bool remove(int);
  TodoTree queryAll();
  Todo query(int);
  bool verifyID(unsigned int id);

private:
  sqlite3 *db;
};

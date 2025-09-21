#include "todo.hpp"
#include <iostream>

int main(int argc, char **argv) {
  TodoStorage TS;
  TS.insert({"test"});
  auto todos = TS.query();
  for (auto todo : todos) {
    std::cout << todo.name << " " << todo.timestamp << std::endl;
  }
  return 0;
}

#pragma once

#include "todo.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Parser {
  using Value =
      std::variant<int *, double *, std::string *, bool *, Todo::Status *>;

  static int parse(const std::string &material, std::vector<Value> &&inputs) {
    auto iss = std::istringstream(material);
    std::string token;
    int count = 0;

    while (iss >> token) {
      if (!parse_token(inputs[count], token)) {
        break;
      }
      count++;
      if (count > inputs.size())
        break;
    }

    return count;
  }

  static bool parse_token(Value &value, const std::string &token) {
    if (std::holds_alternative<double *>(value)) {
      double val;
      try {
        val = std::stod(token);
      } catch (...) {
        std::cout << token << " type(double) mismatch!\n";
        std::cout << token << " ignored ...\n";
        return false;
      }

      *std::get<double *>(value) = val;
      return true;
    }

    if (std::holds_alternative<int *>(value)) {
      int val;
      try {
        val = std::stod(token);
      } catch (...) {
        std::cout << token << " type(int) mismatch!\n";
        std::cout << token << " ignored ...\n";
        return false;
      }

      *std::get<int *>(value) = val;
      return true;
    }

    if (std::holds_alternative<bool *>(value)) {
      bool val;
      if (token == "yes" || token == "y")
        val = true;
      else if (token == "no" || token == "n")
        val = false;
      else {
        std::cout << token << " type(bool) mismatch!\n";
        std::cout << token << " ignored ...\n";
        return false;
      }

      *std::get<bool *>(value) = val;
      return true;
    }

    if (std::holds_alternative<Todo::Status *>(value)) {
      Todo::Status val;
      if (token == "suspend" || token == "s")
        val = Todo::Status::suspend;
      else if (token == "underway" || token == "u")
        val = Todo::Status::underway;
      else if (token == "closed" || token == "c")
        val = Todo::Status::closed;
      else {
        std::cout << token << " type(status) mismatch!\n";
        std::cout << token << " ignored ...\n";
        return false;
      }

      *std::get<Todo::Status *>(value) = val;
      return true;
    }

    if (std::holds_alternative<std::string *>(value)) {
      *std::get<std::string *>(value) = token;
      return true;
    }

    return false;
  }
};

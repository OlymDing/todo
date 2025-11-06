#pragma once
#include <string>
#include <variant>
#include <vector>

using Value = std::variant<int, std::string, float>;

#define REGISTER(cmd)                                                          \
  callbacks[#cmd] = std::bind(&ConsoleUI::cmd, this, std::placeholders::_1);

#define LOG(words) std::cout << __func__ << " >> " << words
#define LOG_MAIN(words) std::cout << "Console >> " << words

// nullable read
#define N_READ(var)                                                            \
  std::string var;                                                             \
  LOG(#var ": ");                                                              \
  std::getline(std::cin, var);

#define READ(var)                                                              \
  N_READ(var)                                                                  \
  if (var.size() == 0) {                                                       \
    LOG("invalid input !\n");                                                  \
    return;                                                                    \
  }

unsigned long long date2timeStamp(std::string &dateStr);
std::string timeStamp2date(time_t timestamp);
std::vector<Value> parse(std::string_view view, int length);

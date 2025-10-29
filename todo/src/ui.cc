#include "ui.hpp"
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>

#define REGISTER(cmd)                                                          \
  callbacks[#cmd] = std::bind(&ConsoleUI::cmd, this, std::placeholders::_1);

#define LOG(words) std::cout << __func__ << " >> " << words
#define LOG_MAIN(words) std::cout << "Console >> " << words

// public
// ======

ConsoleUI::ConsoleUI() { REGISTER(quit); }

void ConsoleUI::loop() {
  while (goNext) {
    LOG_MAIN("");
    std::getline(std::cin, buffer);
    parse();
  }
}

// private
// =======

void ConsoleUI::parse() {
  std::string first_word;
  std::stringstream ss(buffer);
  std::string_view params;

  // get first word
  ss >> first_word;

  // get params
  if (first_word.size() < buffer.size()) {
    params =
        std::string_view(buffer).substr(first_word.size() + 1, buffer.size());
  } else {
    params = "";
  }

  // call the callback if it exists
  if (first_word.length() != 0 && callbacks[first_word])
    callbacks[first_word](params);
  else {
    LOG_MAIN("invalid input !\n");
  }
}

// callbacks
void ConsoleUI::quit(std::string_view params) {
  goNext = false;
  LOG("bye bye ~\n");
}

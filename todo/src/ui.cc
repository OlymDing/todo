#include "ui.hpp"
#include "util.hpp"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

// public
// ======

ConsoleUI::ConsoleUI() {
  REGISTER(quit);
  REGISTER(todo);
  REGISTER(help);
  REGISTER(show);
  REGISTER(remove);
  REGISTER(modify);
}

void ConsoleUI::loop() {
  while (goNext) {
    LOG_MAIN("");
    std::getline(std::cin, buffer);

    // handle ctrl+d
    if (std::cin.eof())
      break;

    parse();
  }
  std::cout << "bye bye ~\n";
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
void ConsoleUI::quit(std::string_view params) { goNext = false; }

void ConsoleUI::help(std::string_view params) {
  LOG("this is help manual...\n");
}

void ConsoleUI::todo(std::string_view params) {
  READ(title)
  N_READ(due_date)

  LOG("title is " << title << " and due_date is " << due_date << '\n');
}

void ConsoleUI::show(std::string_view params) {}

void ConsoleUI::remove(std::string_view params) {}

void ConsoleUI::modify(std::string_view params) {}

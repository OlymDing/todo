#include "ui.hpp"
#include "parser.hpp"
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
  REGISTER(update);
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
  READ(title);
  N_READ(due_date);

  unsigned long long timeStamp = 0;

  if (due_date.size() != 0) {
    timeStamp = date2timeStamp(due_date);
    if (timeStamp == 0)
      LOG("invalid date input, ignored\n");
  }

  TS.insert(title, timeStamp);
}

void ConsoleUI::show(std::string_view params) {
  auto todos = TS.queryAll();
  for (auto &todo : todos) {
    todo.print();
  }
}

void ConsoleUI::remove(std::string_view params) {}

// modify <id> <status = underway> <title = "">
void ConsoleUI::update(std::string_view params) {
  int id;
  Todo::Status status;
  std::string title = "";

  auto count = Parser::parse(std::string(params), {&id, &status, &title});

  if (count > 1) {
    // at least id and status are provided
    auto todo = TS.query(id);

    if (!todo.isValid) {
      std::cout << "no such id !\n";
      return;
    }

    todo.status = status;
    if (title != "")
      todo.name = title;

    TS.update(todo);
  } else {
    std::cout << "invalid params !\n";
  }
}

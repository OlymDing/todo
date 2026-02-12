#include "cli.hpp"
#include "parser.hpp"
#include "storage.hpp"
#include "util.hpp"
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <readline/readline.h>
#include <readline/history.h>

// public
// ======

CLI::CLI()
{
  REGISTER(quit);
  REGISTER(exit);
  REGISTER(todo);
  REGISTER(subtodo);
  REGISTER(help);
  REGISTER(show);
  REGISTER(remove);
  REGISTER(update);
}

void CLI::loop()
{
  show("");
  while (goNext)
  {
    char *line = readline("Console >> ");
    if (line == NULL)
    {
      break;
    }

    buffer = std::string(line);
    parse();
  }
  std::cout << "bye bye ~\n";
}

// private
// =======

void CLI::parse()
{
  std::string first_word;
  std::stringstream ss(buffer);
  std::string_view params;

  // get first word
  ss >> first_word;

  // get params
  if (first_word.size() < buffer.size())
  {
    params =
        std::string_view(buffer).substr(first_word.size() + 1, buffer.size());
  }
  else
  {
    params = "";
  }

  // call the callback if it exists
  if (first_word.length() != 0 && callbacks[first_word])
    callbacks[first_word](params);
  else
  {
    LOG_MAIN("invalid input !\n");
  }
}

// callbacks
void CLI::quit(std::string_view params) { goNext = false; }
void CLI::exit(std::string_view params) { goNext = false; }

void CLI::help(std::string_view params)
{
  LOG("this is help manual...\n");
}

void CLI::todo(std::string_view params)
{
  READ(title);
  N_READ(due_date);

  unsigned long long timeStamp = 0;
  if (due_date.size() != 0)
  {
    timeStamp = date2timeStamp(due_date);
    if (timeStamp == 0)
      LOG("invalid date input, ignored\n");
  }
  TS.insert(title, 0, timeStamp);
}

void CLI::subtodo(std::string_view params)
{
  READ(title);
  READ(parentId);
  N_READ(due_date);

  int intParentId = 0;
  unsigned long long timeStamp = 0;

  // process parentId
  try
  {
    intParentId = std::stoi(parentId);
    if (!TS.verifyID(intParentId))
    {
      LOG("no such parentId !\n");
      return;
    }
  }
  catch (std::exception e)
  {
    LOG("invalid parentId !\n");
    return;
  }

  // process due time
  if (due_date.size() != 0)
  {
    timeStamp = date2timeStamp(due_date);
    if (timeStamp == 0)
      LOG("invalid date input, ignored\n");
  }

  TS.insert(title, intParentId, timeStamp);
}

void CLI::show(std::string_view params)
{
  int id;
  auto count = Parser::parse(std::string(params), {&id});

  Todo::printHeaders();
  if (count == 0)
  {
    auto todoTree = TS.queryAll();
    std::cout << todoTree.summary();
    // for (auto &todo : todos) {
    //   todo.print();
    // }
  }
  else if (count == 1)
  {
    auto todo = TS.query(id);
    if (todo.mIsValid)
      todo.print();
    else
      LOG("no such id !\n");
  }
}

void CLI::remove(std::string_view params)
{
  int id;
  auto count = Parser::parse(std::string(params), {&id});

  if (count != 1)
  {
    LOG("invalid param!\n");
  }

  auto todo = TS.query(id);
  if (todo.mIsValid)
  {
    LOG("sure to delete this? (y/n)\n");
    todo.print();

    READ(confirm);
    if (confirm == "y")
    {
      TS.remove(id);
      LOG("id " << id << " removed\n");
    }
    else
      LOG("cancelled\n");
  }
  else
    LOG("no such id !\n");
}

// modify <id> <status = underway> <title = "">
void CLI::update(std::string_view params)
{
  int id;

  auto count = Parser::parse(std::string(params), {&id});

  if (count == 1)
  {
    auto todo = TS.query(id);

    if (!todo.mIsValid)
    {
      LOG("no such id !\n");
      return;
    }

    READ(status);
    if (status == "underway")
      todo.mStatus = Todo::underway;
    else if (status == "suspend")
      todo.mStatus = Todo::suspend;
    else if (status == "closed")
      todo.mStatus = Todo::closed;

    TS.update(todo);
  }
  else
  {
    LOG("invalid params !\n");
  }
}

#include "todo.hpp"
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

class ConsoleUI {
public:
  ConsoleUI();
  void loop();

private:
  void parse();

  std::string buffer;
  TodoStorage TS;
  bool goNext = true;

  // callbacks
  std::unordered_map<std::string, std::function<void(std::string_view)>>
      callbacks;
  void quit(std::string_view params);
  void todo(std::string_view params);
  void help(std::string_view params);
  void show(std::string_view params);
  void remove(std::string_view params);
  void modify(std::string_view params);
};

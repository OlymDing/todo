#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

// 通过宏来定义一个
// #define CMD(cmd)

class ConsoleUI {
public:
  ConsoleUI();
  void loop();

private:
  void parse(); // 解析指令

  std::string buffer;
  // char buffer[1000];  // 保存每一行指令
  bool goNext = true; // 是否继续执行
  std::unordered_map<std::string, std::function<void(std::string_view)>>
      callbacks; // 通过指令直接拿到具体的回调函数

  // callbacks
  void quit(std::string_view params);
  void insert(std::string_view params);
  void help(std::string_view params);
  void show(std::string_view params);
  void remove(std::string_view params);
  void modify(std::string_view params);
};

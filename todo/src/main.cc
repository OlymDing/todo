// #include "ui.hpp"

// int main(int argc, char **argv)
// {
//   ConsoleUI ui;
//   ui.loop();
//   return 0;
// }

#include "PainterEngine.h"
#include <iostream>
#include "gui.hpp"
#include "ui.hpp"

int main()
{
  std::cout << "mode: ";
  std::string mode;
  std::cin >> mode;

  if (mode == "tui")
  {
    ConsoleUI ui;
    ui.loop();
    return 0;
  }
  else if (mode == "gui")
  {
    auto gui = GUI();
    gui.show();
    std::cout << "test\n";
    return 0;
  }
  return 1;
}

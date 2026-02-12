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
#include "cli.hpp"

int main()
{
  std::cout << "mode: ";
  std::string mode;
  std::cin >> mode;

  if (mode == "cli")
  {
    CLI ui;
    ui.loop();
    return 0;
  }
  else if (mode == "gui")
  {
    GUI ui;
    ui.show();
    return 0;
  }
  return 1;
}

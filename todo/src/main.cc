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
  // ConsoleUI ui;
  // ui.loop();
  // return 0;
  auto gui = GUI();
  gui.show();
  std::cout << "test\n";
  return 1;
}

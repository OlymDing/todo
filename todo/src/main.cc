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

int main()
{
  auto gui = GUI();
  gui.loop();
  std::cout << "test\n";
  return 1;
}

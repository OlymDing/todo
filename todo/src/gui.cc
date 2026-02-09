#include "gui.hpp"
#include <PainterEngine.h>

GUI::GUI()
{
  PainterEngine_Initialize(800, 480);
  PainterEngine_LoadFontModule(
      "/home/olym/Downloads/CascadiaMono/CaskaydiaMonoNerdFont-Bold.ttf", PX_FONTMODULE_CODEPAGE_GBK, 20
  );
}

void GUI::loop()
{
  PX_Object *card = PX_Object_TodoCardCreate(mp, root, 0, 0);
}

#include "gui.hpp"
#include <PainterEngine.h>

PX_OBJECT_EVENT_FUNCTION(OnButtonClick)
{
  PX_Object_PushButtonSetText(pObject, "item clicked !");
}

GUI::GUI()
{
  PainterEngine_Initialize(800, 480);
  PX_Object *myButtonObject;
  PainterEngine_LoadFontModule(
      "assets/font.ttf", PX_FONTMODULE_CODEPAGE_GBK, 20
  );
  myButtonObject = PX_Object_PushButtonCreate(
      mp, root, 300, 200, 200, 80, "hello painterEngine",
      PainterEngine_GetFontModule()
  );
  PX_ObjectRegisterEvent(
      myButtonObject, PX_OBJECT_EVENT_EXECUTE, OnButtonClick, 0
  );
}

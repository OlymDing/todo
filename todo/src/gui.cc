#include "gui.hpp"
#include "kernel/PX_Object_Label.h"
#include <PainterEngine.h>

// gui
// ===

GUI::GUI()
{
  PainterEngine_Initialize(800, 480);
  PainterEngine_LoadFontModule(
      "/home/olym/Downloads/CascadiaMono/CaskaydiaMonoNerdFont-Bold.ttf",
      PX_FONTMODULE_CODEPAGE_GBK, 20
  );
}

void GUI::show()
{
  auto todoTree = TS.queryAll();
  auto node = todoTree.mList.front();
  auto *card = new Card(0, 0);
  card->updateTitle(node->value->mName);
}

// card
// ====

px_void CardOnDrag(PX_Object *pObject, PX_Object_Event e, px_void *ptr)
{
  pObject->x = PX_Object_Event_GetCursorX(e);
  pObject->y = PX_Object_Event_GetCursorY(e);
}

PX_OBJECT_UPDATE_FUNCTION(TodoCardUpdate) {}

PX_OBJECT_RENDER_FUNCTION(TodoCardRender)
{
  px_rect rect = PX_ObjectGetRect(pObject);
  auto x = (px_int)rect.x;
  auto y = (px_int)rect.y;
  auto w = (px_int)rect.width;
  auto h = (px_int)rect.height;
  PX_GeoDrawBorder(psurface, x, y, x + w, y + h - 1, 1, PX_COLOR_BLACK);
}
PX_OBJECT_FREE_FUNCTION(TodoCardFree) {}

PX_Object *PX_Object_TodoCardCreate(
    px_memorypool *mp, PX_Object *parent, px_float x, px_float y
)
{
  PX_Object *pObject = PX_ObjectCreateEx(
      mp, parent, x, y, 0, 220, 100, 0, 0, TodoCardUpdate, TodoCardRender,
      TodoCardFree, 0, sizeof(TodoCard)
  );
  TodoCard *card = PX_ObjectGetDescIndex(TodoCard, pObject, 0);
  card->title_header = PX_Object_LabelCreate(
      mp, pObject, x + 10, y + 10, 90, 40,
      "title: ", PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->title_header, true);

  card->title_text = PX_Object_LabelCreate(
      mp, pObject, x + 110, y + 10, 100, 40, "title text",
      PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->title_text, true);

  card->status_header = PX_Object_LabelCreate(
      mp, pObject, x + 10, y + 50, 90, 40,
      "status: ", PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->status_header, true);

  card->status_text = PX_Object_LabelCreate(
      mp, pObject, x + 110, y + 50, 100, 40, "status text",
      PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );

  PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORDRAG, CardOnDrag, 0);
  return pObject;
}

void Card::updateTitle(const std::string &title)
{
  PX_Object_LabelSetText(card->title_text, title.c_str());
}

void Card::updateStatus(const std::string &status)
{
  PX_Object_LabelSetText(card->status_text, status.c_str());
}

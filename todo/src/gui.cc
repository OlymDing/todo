#include "gui.hpp"
#include "kernel/PX_Object.h"
#include "kernel/PX_Object_Label.h"
#include <PainterEngine.h>

// gui
// ===

GUI::GUI()
{
  PainterEngine_Initialize(1800, 1480);
  PainterEngine_LoadFontModule(
      "/home/olym/otherProjects/popular-fonts/microsoft.ttf",
      PX_FONTMODULE_CODEPAGE_UTF8, 20
  );
}

void GUI::show()
{
  int x = 0, y = 0;
  auto todoTree = TS.queryAll();
  auto node = todoTree.mList.front();
  todoTree.traversal([this](Todo *todo, int row, int col)
                     { createCard(todo, row, col); });
}

void GUI::createCard(Todo *todo, int row, int col)
{
  int x = (col - 1) * 230;
  int y = (row - 1) * 110;

  auto card = new Card(x, y);
  card->updateTitle(todo->mName);
  cards.push_back(card);
}

// card
// ====

PX_OBJECT_EVENT_FUNCTION(CardOnDrag)
{
  TodoCard *card = PX_ObjectGetDescIndex(TodoCard, pObject, 0);
  if (card->bselect)
  {
    pObject->x += PX_Object_Event_GetCursorX(e) - card->last_cursorx;
    pObject->y += PX_Object_Event_GetCursorY(e) - card->last_cursory;
  }
  card->last_cursorx = PX_Object_Event_GetCursorX(e);
  card->last_cursory = PX_Object_Event_GetCursorY(e);
}

PX_OBJECT_EVENT_FUNCTION(CardOnMouseDown)
{
  TodoCard *card = PX_ObjectGetDescIndex(TodoCard, pObject, 0);
  if (PX_ObjectIsCursorInRegion(pObject, e))
  {
    card->bselect = PX_TRUE;
    card->last_cursorx = PX_Object_Event_GetCursorX(e);
    card->last_cursory = PX_Object_Event_GetCursorY(e);
  }
}

PX_OBJECT_EVENT_FUNCTION(CardOnMouseRelease)
{
  TodoCard *card = PX_ObjectGetDescIndex(TodoCard, pObject, 0);
  card->bselect = false;
}

PX_OBJECT_EVENT_FUNCTION(CardOnMouseWheel)
{
  pObject->y += (px_float)PX_Object_Event_GetCursorZ(e) * 10;
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
      mp, pObject, 10, 10, 90, 40, "title: ", PainterEngine_GetFontModule(),
      PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->title_header, true);

  card->title_text = PX_Object_LabelCreate(
      mp, pObject, 110, 10, 100, 40, "title text",
      PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->title_text, true);

  card->status_header = PX_Object_LabelCreate(
      mp, pObject, 10, 50, 90, 40, "status: ", PainterEngine_GetFontModule(),
      PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->status_header, true);

  card->status_text = PX_Object_LabelCreate(
      mp, pObject, 110, 50, 100, 40, "status text",
      PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );

  PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORDRAG, CardOnDrag, 0);
  PX_ObjectRegisterEvent(
      pObject, PX_OBJECT_EVENT_CURSORDOWN, CardOnMouseDown, 0
  );
  PX_ObjectRegisterEvent(
      pObject, PX_OBJECT_EVENT_CURSORUP, CardOnMouseRelease, 0
  );
  PX_ObjectRegisterEvent(
      pObject, PX_OBJECT_EVENT_CURSORWHEEL, CardOnMouseWheel, 0
  ); // 注册鼠标滚轮事件
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

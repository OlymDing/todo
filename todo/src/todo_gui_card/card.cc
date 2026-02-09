#include <PainterEngine.h>
#include "core/PX_Typedef.h"
#include "gui.hpp"
#include "kernel/PX_Object_Label.h"

PX_OBJECT_EVENT_FUNCTION(OnButtonClick)
{
  PX_Object_PushButtonSetText(pObject, "item clicked !");
}

typedef struct
{
  PX_Object *title_header, *title_text;
  PX_Object *status_header, *status_text;
} TodoCard;

PX_OBJECT_UPDATE_FUNCTION(TodoCardUpdate)
{
  // todo
}
PX_OBJECT_RENDER_FUNCTION(TodoCardRender)
{

  px_rect rect = PX_ObjectGetRect(pObject);
  auto x = (px_int)rect.x;
  auto y = (px_int)rect.y;
  auto w = (px_int)rect.width;
  auto h = (px_int)rect.height;

  PX_GeoDrawBorder(psurface, x, y, x + w, y + h - 1, 1, PX_COLOR_BLACK);
  // TodoCard *card = PX_ObjectGetDesc(TodoCard, pObject);
  // todo
}
PX_OBJECT_FREE_FUNCTION(TodoCardFree)
{
  // todo
}

PX_Object *PX_Object_TodoCardCreate(
    px_memorypool *mp, PX_Object *parent, px_float x, px_float y
)
{
  PX_Object *pObject = PX_ObjectCreateEx(
      mp, parent, x, y, 0, 220, 100, 0, 0, TodoCardUpdate, TodoCardRender,
      TodoCardFree, 0, sizeof(TodoCard)
  );
  TodoCard *card = PX_ObjectGetDescIndex(TodoCard, pObject, 0);

  // card->left_button = PX_Object_PushButtonCreate(
  //     mp, pObject, 100, 0, 200, 40, "hello left",
  //     PainterEngine_GetFontModule()
  // );
  // card->right_button = PX_Object_PushButtonCreate(
  //     mp, pObject, 100, 39, 200, 40, "hello right",
  //     PainterEngine_GetFontModule()
  // );
  card->title_header = PX_Object_LabelCreate(
      mp, root, x + 10, y + 10, 90, 40,
      "title: ", PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->title_header, true);

  card->title_text = PX_Object_LabelCreate(
      mp, root, x + 110, y + 10, 100, 40, "title text",
      PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->title_text, true);

  card->status_header = PX_Object_LabelCreate(
      mp, root, x + 10, y + 50, 90, 40,
      "status: ", PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->status_header, true);

  card->status_text = PX_Object_LabelCreate(
      mp, root, x + 110, y + 50, 100, 40, "status text",
      PainterEngine_GetFontModule(), PX_COLOR_BLACK
  );
  // PX_Object_LabelSetBorder(card->status_text, true);
  return pObject;
}

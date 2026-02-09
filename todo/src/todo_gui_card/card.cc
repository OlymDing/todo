#include <PainterEngine.h>
#include "gui.hpp"

PX_OBJECT_EVENT_FUNCTION(OnButtonClick)
{
  PX_Object_PushButtonSetText(pObject, "item clicked !");
}

typedef struct
{
  PX_Object *left_button, *right_button;
} TodoCard;

PX_OBJECT_UPDATE_FUNCTION(TodoCardUpdate)
{
  // todo
}
PX_OBJECT_RENDER_FUNCTION(TodoCardRender)
{
  TodoCard *card = PX_ObjectGetDesc(TodoCard, pObject);
  PX_ObjectRender(psurface, card->left_button, 0);
  PX_ObjectRender(psurface, card->right_button, 0);
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
      mp, parent, x, y, 0, 128, 128, 0, 0, TodoCardUpdate, TodoCardRender,
      TodoCardFree, 0, sizeof(TodoCard)
  );
  TodoCard *card = PX_ObjectGetDescIndex(TodoCard, pObject, 0);
  card->left_button = PX_Object_PushButtonCreate(
      mp, pObject, 300, 200, 200, 40, "hello left",
      PainterEngine_GetFontModule()
  );
  card->right_button = PX_Object_PushButtonCreate(
      mp, pObject, 300, 239, 200, 40, "hello right",
      PainterEngine_GetFontModule()
  );
  PX_ObjectRegisterEvent(
      card->right_button, PX_OBJECT_EVENT_EXECUTE, OnButtonClick, 0
  );
  PX_ObjectRegisterEvent(
      card->left_button, PX_OBJECT_EVENT_EXECUTE, OnButtonClick, 0
  );
  return pObject;
}

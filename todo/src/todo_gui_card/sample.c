#include "PainterEngine.h"
typedef struct
{
  px_texture image;
  px_int rotation;
} PX_Object_MyObject;

PX_OBJECT_UPDATE_FUNCTION(MyObjectUpdate) {}

PX_OBJECT_RENDER_FUNCTION(MyObjectRender)
{
  PX_Object_MyObject *pMyObject = PX_ObjectGetDesc(PX_Object_MyObject, pObject);
  PX_TextureRenderEx(
      psurface, &pMyObject->image, (px_int)pObject->x, (px_int)pObject->y,
      PX_ALIGN_CENTER, 0, 1, pMyObject->rotation
  );
}

PX_OBJECT_FREE_FUNCTION(MyObjectFree)
{
  PX_Object_MyObject *pMyObject = PX_ObjectGetDesc(PX_Object_MyObject, pObject);
  PX_TextureFree(&pMyObject->image);
}

PX_Object *PX_Object_MyObjectCreate(
    px_memorypool *mp, PX_Object *parent, px_float x, px_float y
)
{
  PX_Object *pObject = PX_ObjectCreateEx(
      mp, parent, x, y, 0, 128, 128, 0, 0, MyObjectUpdate, MyObjectRender,
      MyObjectFree, 0, sizeof(PX_Object_MyObject)
  );
  PX_Object_MyObject *pMyObject =
      PX_ObjectGetDescIndex(PX_Object_MyObject, pObject, 0);
  pMyObject->rotation = 50;
  if (!PX_LoadTextureFromFile(
          mp, &pMyObject->image,
          "/home/olym/otherProjects/PainterEngine/documents/assets/img/4.1.png"
      ))
  {
    PX_ObjectDelete(pObject);
    return PX_NULL;
  }
  return pObject;
}

px_int main()
{
  PainterEngine_Initialize(800, 480);
  PX_Object_MyObjectCreate(mp, root, 400, 240);
  return PX_TRUE;
}

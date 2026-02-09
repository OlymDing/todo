#pragma once
#include <PainterEngine.h>

class GUI
{
public:
  GUI();
  ~GUI() {}

  void loop();
};

PX_Object *PX_Object_TodoCardCreate(
    px_memorypool *mp, PX_Object *parent, px_float x, px_float y
);

#pragma once
#include <PainterEngine.h>
#include "todo.hpp"

class GUI
{
public:
  TodoStorage TS;

  GUI();
  ~GUI() {}

  void show();
};

PX_Object *PX_Object_TodoCardCreate(
    px_memorypool *mp, PX_Object *parent, px_float x, px_float y
);

struct TodoCard
{
  PX_Object *title_header, *title_text;
  PX_Object *status_header, *status_text;
};

struct Card
{
  PX_Object *cardObject;
  TodoCard *card;

  Card(float x, float y)
  {
    cardObject = PX_Object_TodoCardCreate(mp, root, x, y);
    card = PX_ObjectGetDescIndex(TodoCard, cardObject, 0);
  }
  ~Card() {}

  void updateTitle(const std::string &title);
  void updateStatus(const std::string &status);
};

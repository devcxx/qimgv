#ifndef SHAPEDRAWER_H
#define SHAPEDRAWER_H

#include "drawer.h"
#include "../core/edit_core.h"

class ShapeDrawer : public Drawer
{
public:
    ShapeDrawer(const shape_object& object,QPainter& p,QObject *parent=nullptr);
};

#endif

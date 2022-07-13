#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include "drawer.h"
#include "../core/edit_core.h"

class TextDrawer : public Drawer
{
public:
    TextDrawer(const shape_object& object,QPainter &p,QRect selection,QObject *parent=nullptr);
};

#endif

#ifndef RECTANGLEBLURDRAWER_H
#define RECTANGLEBLURDRAWER_H

#include "drawer.h"

class RectangleBlurDrawer : public Drawer
{
public:
    explicit RectangleBlurDrawer(const QPixmap& pixmap,const QRect&rect, const QRect&region, QPainter&p,qreal r=10, QObject* parent=nullptr);
    int m_blockSize;
};

#endif

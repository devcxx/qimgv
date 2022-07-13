#include "textdrawer.h"

TextDrawer::TextDrawer(const shape_object &object, QPainter &p,QRect selection, QObject *parent):
    Drawer(parent)
{
    p.save();
    p.setFont(object.font);
    p.setPen(object.color);
    QFontMetrics fm(object.font);
    for(int j=0; j<object.ts.size(); j++){
        //auto r = QRect(object.ps[0].x(), object.ps[j].y() + j * fm.lineSpacing(), fm.width(object.ts[j]), fm.lineSpacing()).intersected(selection);
        //p.drawText(object.ps[j],object.ts[j]);
        auto r = QRect(object.ps[0].x(), object.ps[0].y() + (fm.height() + fm.leading()) * j, fm.width(object.ts[j]), fm.height() + fm.leading()).intersected(selection);
        p.drawText(r, object.ts[j]);
    }
    p.restore();
}
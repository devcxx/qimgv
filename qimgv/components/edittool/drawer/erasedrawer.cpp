#include "erasedrawer.h"

EraseDrawer::EraseDrawer(const shape_object &object, QPainter &p,QPainter& pt,const QRect&region, QObject *parent)
    :Drawer(parent)
{
    if(object.egeneral){
        int eraseSize=object.erasesize;
        for(auto && x:object.ps){
            QRect r(x.x()-eraseSize/2,x.y()-eraseSize/2,eraseSize,eraseSize);
            p.eraseRect(r);
            auto old=pt.compositionMode();
            pt.setCompositionMode(QPainter::CompositionMode_Clear);
            pt.eraseRect(r);
            pt.setCompositionMode(old);
        }
    }else{
        p.eraseRect(object.rs[0].normalized().adjusted(-2, -2, 2, 2));
        auto old=pt.compositionMode();
        QRect rect = object.rs[0];
        int width = rect.width();
        int height = rect.height();
        QRect selection = QRect(rect.x()-region.x(), rect.y()-region.y(), width, height);

        pt.setCompositionMode(QPainter::CompositionMode_Clear);
        pt.eraseRect(selection);
        pt.setCompositionMode(old);
    }
}

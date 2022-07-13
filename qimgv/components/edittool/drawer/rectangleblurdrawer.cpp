#include "rectangleblurdrawer.h"

#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

RectangleBlurDrawer::RectangleBlurDrawer(const QPixmap &pixmap, const QRect &rect, const QRect&region, QPainter &p, qreal r, QObject *parent)
    :Drawer(parent)
    , m_blockSize(8)
{
    QPoint p0=rect.topLeft();
    QPoint p1=rect.bottomRight();
//    QRect selection = QRect(p0,p1).normalized();


    int width = rect.width();
    int height = rect.height();
    QRect selection = QRect(rect.x()-region.x(), rect.y()-region.y(), width, height);

    QPixmap imagePixelate = pixmap.copy(selection);


    qreal sw = 1.0 / m_blockSize;
    qreal sh = 1.0 / m_blockSize;

    imagePixelate = imagePixelate.scaled(int(sw * imagePixelate.width()),
                                     int(sh * imagePixelate.height()),
                                     Qt::IgnoreAspectRatio,
                                     Qt::FastTransformation)
                        .scaled(imagePixelate.width(),
                            imagePixelate.height(),
                            Qt::IgnoreAspectRatio,
                            Qt::FastTransformation);


    p.drawPixmap(QRect(p0,p1).normalized(), imagePixelate);


//    auto pixelRatio = pixmap.devicePixelRatio();
//    QRect selectionScaled =
//    QRect(p0 * pixelRatio, p1 * pixelRatio).normalized();

//    int width = static_cast<int>(selection.width() *
//        (0.5 / qMax(1, m_blockSize + 1)));
//    int height = static_cast<int>(selection.height() *
//        (0.5 / qMax(1, m_blockSize + 1)));
//    QSize size = QSize(qMax(width, 1), qMax(height, 1));

//    QPixmap t = pixmap.copy(selectionScaled);
//    t = t.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    t = t.scaled(selection.width(), selection.height());

//    p.drawPixmap(selection, t);

//    QGraphicsScene scene;
//    QGraphicsPixmapItem item;

//    qreal pixelRatio=pixmap.devicePixelRatio();
//    QRect selectionScaled=QRect(p0 * pixelRatio, p1 * pixelRatio).normalized();
//    QGraphicsBlurEffect *blur=new QGraphicsBlurEffect(this);
//    blur->setBlurRadius(r);
//    item.setPixmap(pixmap.copy(selectionScaled));
//    item.setGraphicsEffect(blur);
//    scene.addItem(&item);
//    scene.render(&p,selection);
}


#include "pinwidget.h"
#include "../core/core_system.h"
#include <QKeyEvent>
#include <QPainter>
#include <QShortcut>
#include <qmath.h>

PinWidget::PinWidget(const QPixmap &pixmap, QWidget *parent) : QWidget(parent),__pixmap(pixmap),
    wheelFactor(0.9)
{
    setWindowTitle(tr("Pin Window"));
    setWindowIcon(QIcon(":/svg/res/images/logo.svg"));
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    new QShortcut(QKeySequence(Qt::Key_Escape),this,SLOT(closePinWidget()));
    borderColor=qCore->getHoverColor();
    QSize size=qCore->getScreenSize();
    move((size.width()-width())/2,(size.height()-height())/2);
}
void PinWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(rect(),__pixmap);
    p.setPen(QPen(borderColor,4));
    p.drawRect(rect());
    QWidget::paintEvent(e);
}
void PinWidget::enterEvent(QEvent *)
{
    setCursor(Qt::SizeAllCursor);
    borderColor=qCore->getHoverColor();
    update();
}
void PinWidget::leaveEvent(QEvent *)
{
    setCursor(Qt::ArrowCursor);
    borderColor=qCore->getCheckedColor();
    update();
}

void PinWidget::wheelEvent(QWheelEvent *event)
{
    if (wheelFactor != 0.0 )
    {
        /*
            A positive delta indicates that the wheel was
            rotated forwards away from the user; a negative
            value indicates that the wheel was rotated
            backwards toward the user.
            Most mouse types work in steps of 15 degrees,
            in which case the delta value is a multiple
            of 120 (== 15 * 8).
         */
        double f = qPow( wheelFactor,
            qAbs( event->delta() / 120.0 ) );

        if ( event->delta() > 0 )
            f = 1 / f;
        resize(width()*f, height()*f);
    }
}
void PinWidget::mousePressEvent(QMouseEvent *e)
{
    offset=e->pos();
}
void PinWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::MouseButton::LeftButton){
        QPoint pos=e->pos();
        move(mapToGlobal(QPoint(pos.x()-offset.x(),pos.y()-offset.y())));
    }
}
void PinWidget::closePinWidget()
{
    close();
    emit closed();
}

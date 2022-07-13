#include "basebutton.h"
#include <QGraphicsDropShadowEffect>
#include "settings.h"
#include "utils/imagelib.h"

extern Settings *settings;

BaseButton::BaseButton(const QPixmap& pixmap,const QString& tooltip,QWidget *parent,bool checkable)
    : QPushButton(parent)
{
    __parent=parent;

    setCheckable(checkable);

    QString styleSheet="BaseButton{border:none;background-color:%1}"
                       "BaseButton:hover{background-color:%2}"
                       "BaseButton:pressed{background-color:%3}"
                       "BaseButton:checked{background-color:%4}";
    setStyleSheet(styleSheet
                      .arg(qCore->getNormalColor().name(QColor::HexArgb))
                  .arg(qCore->getHoverColor().name(QColor::HexArgb))
                  .arg(qCore->getPressedColor().name(QColor::HexArgb))
                  .arg(qCore->getCheckedColor().name(QColor::HexArgb)));


    QPixmap recolor(pixmap);
    ImageLib::recolor(recolor, settings->colorScheme().icons);
    setSVGIcon(recolor);

    setToolTip(tooltip);
    enableShadowEffect();
}

BaseButton::BaseButton(const QString &pixmapUrl,const QString& tooltip, QWidget *parent, bool checkable)
    : BaseButton(QPixmap(pixmapUrl),tooltip,parent,checkable)
{}

void BaseButton::enableShadowEffect()
{
    if(qCore->getShadowEffect()){
        auto dsEffect = new QGraphicsDropShadowEffect(this);
        dsEffect->setBlurRadius(5);
        dsEffect->setOffset(0);
        dsEffect->setColor(QColor(Qt::black));
        setGraphicsEffect(dsEffect);
    }
}

void BaseButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    __prevCursor=cursor();
    setCursor(Qt::ArrowCursor);
}

void BaseButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    setCursor(__prevCursor);
}

void BaseButton::setSVGIcon(const QIcon &icon)
{
    setIcon(icon);
    setIconSize(QSize(width(),__parent->height()-qCore->borderPadding()));
}


#ifndef PIXELPANEL_H
#define PIXELPANEL_H

#include <QWidget>
#include <QKeyEvent>

class EditViewer;
class PixelWidget;
class QLabel;

class PixelPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PixelPanel(QWidget *parent = nullptr);
    ~PixelPanel();
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *e);
    void setPos(const QPoint &);
    QString convertHEXColor();

    bool __copy=false;
    bool __convertRGBHEX=false;

    void initUI();

private:
    EditViewer *__w;
    PixelWidget * pixelWidget;

    QLabel *lbPOS;
    QLabel *lbMSG;
    QLabel *lbMSG2;
    QLabel *lbColor;
    QLabel *lbRGB;

    QRgb rgb;
    QPoint point;
};

#endif

#include "editeviewer.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QShortcut>
#include <QStyleOption>

#include "drawer/arrowdrawer.h"
#include "drawer/erasedrawer.h"
#include "drawer/imageblurdrawer.h"
#include "drawer/pendrawer.h"
#include "drawer/rectangleblurdrawer.h"
#include "drawer/shapedrawer.h"
#include "drawer/textdrawer.h"

#include "widget/customtextedit.h"
#include "widget/pixelwidget.h"

#include "settings.h"

extern Settings *settings;
EditViewer::EditViewer(QWidget* parent)
    : QWidget(parent)
    , m_bEdit(true)
    , m_bCanWheel(false)
    , m_ZoomValue(1.0)
    , m_XPtInterval(0)
    , m_YPtInterval(0)
    , d(0)
    , pressed(false)
    , completed(false)
    , painting(false)
    , fill(false)
    , save(false)
    , resizing(true)
    , egeneral(true)
{
    initShortcuts();
    init();
}

EditViewer::~EditViewer()
{
}

void EditViewer::init()
{
    installEventFilter(this);
    setMouseTracking(true);
    QScreen* screen = qCore->getPrimaryScreen();
    screenSize = screen->size();
    qDebug() << QString("Screen Size:(%1,%2)").arg(screenSize.width()).arg(screenSize.height());
//    setAttribute(Qt::WA_TranslucentBackground, true);
#ifdef Q_OS_WIN
    setWindowFlags(Qt::FramelessWindowHint);
    // 暂时注释掉，放开会导致每次查看图片都会产生QWidget::paintEngine: Should no longer be called错误
    // setAttribute(Qt::WA_PaintOnScreen, true);
#endif
    inputTextEdit = new InputTextEdit(this);
    inputTextEdit->hide();

    m_edit = new TextEdit(this);
    m_edit->hide();

    shapeTool = new ShapeTool(this);
    shapeTool->hide();

    toolWidget = new ToolWidget(this);

    pixelPanel = new PixelPanel(this);
    pixelPanel->hide();

    penTool = new PenTool(this);
    penTool->hide();

    textTool = new TextTool(this);
    textTool->hide();

    eraseTool = new EraseTool(this);
    eraseTool->hide();

    arrowTool = new ArrowTool(this);
    arrowTool->hide();

    brushTool = new BrushTool(this);
    brushTool->hide();

    eraseSize = qCore->getEraseSize();
    eraseRegion.setWidth(eraseSize);
    eraseRegion.setHeight(eraseSize);

    resetPaintInfo();

    paintEventNotify = PaintEventNotify::Unknown;
    shapeType = ShapeType::Unknown;
    setCursor(Qt::ArrowCursor);

    setFont(QFont(qCore->getFontFamily()));
}

void EditViewer::initShortcuts()
{
    //    new QShortcut(QKeySequence(Qt::Key_Escape),this,SLOT(cancel()));
    new QShortcut(QKeySequence(Qt::Key_C), this, SLOT(copyColorValue()));
    new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_A), this, SLOT(changeRgb2Hex()));

    new QShortcut(QKeySequence(Qt::Key_Up), this, SLOT(upMovePixelPanel()));
    new QShortcut(QKeySequence(Qt::Key_Down), this, SLOT(downMovePixelPanel()));
    new QShortcut(QKeySequence(Qt::Key_Left), this, SLOT(leftMovePixelPanel()));
    new QShortcut(QKeySequence(Qt::Key_Right), this, SLOT(rightMovePixelPanel()));

    //     new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Up),this,SLOT(upMove()));
    //     new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Down),this,SLOT(downMove()));
    //     new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Left),this,SLOT(leftMove()));
    //     new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Right),this,SLOT(rightMove()));
}

void EditViewer::setEditMode(bool editMode, EditViewer::Mode mode)
{
    m_bEdit = editMode;
    m_editMode = mode;
    qCore->setImageReadonly(mode == Mode::Mode_Readonly ? true : false);
    if (editMode) {
        toolWidget->show();
        setZoomValue();
    } else {
        toolWidget->hide();
    }
}

void EditViewer::copyColorValue()
{
    qDebug() << "Copy color value";
    pixelPanel->__copy = true;
    pixelPanel->update();
}

void EditViewer::cancel()
{
    qDebug() << "Close capture window";
    //    kCapture_window_load=false;
    qCore->resetColor();
    close();
}

void EditViewer::upMovePixelPanel()
{
    if (!region.contains(QCursor::pos()))
        return;
    if (painting)
        return;
    cursorPos.setY(cursorPos.y() - 1);
    movePixelPanel(cursorPos);
}
void EditViewer::downMovePixelPanel()
{
    if (!region.contains(QCursor::pos()))
        return;
    if (painting)
        return;
    cursorPos.setY(cursorPos.y() + 1);
    movePixelPanel(cursorPos);
}
void EditViewer::leftMovePixelPanel()
{
    if (!region.contains(QCursor::pos()))
        return;
    if (painting)
        return;
    cursorPos.setX(cursorPos.x() - 1);
    movePixelPanel(cursorPos);
}
void EditViewer::rightMovePixelPanel()
{
    if (!region.contains(QCursor::pos()))
        return;
    if (painting)
        return;
    cursorPos.setX(cursorPos.x() + 1);
    movePixelPanel(cursorPos);
}

void EditViewer::upMove()
{
    //region.setTop(region.top() - 1);
    moveToolWidget();
    update();
}
void EditViewer::downMove()
{
    //region.setBottom(region.bottom() + 1);s
    moveToolWidget();
    update();
}
void EditViewer::leftMove()
{
    //region.setLeft(region.left() - 1);
    moveToolWidget();
    update();
}
void EditViewer::rightMove()
{
    //region.setRight(region.right() + 1);
    moveToolWidget();
    update();
}

void EditViewer::changeRgb2Hex()
{
    qDebug() << "Convert RGB to HEX";
    pixelPanel->__convertRGBHEX = !pixelPanel->__convertRGBHEX;
    pixelPanel->update();
}

void EditViewer::setImage(const QImage& image)
{
    m_Image = image;
    completed = false;
    sos.clear();
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    setZoomValue();
    this->update();
}

void EditViewer::setImagePath(const QString& path)
{
    m_imgPath = path;
    qCore->setWritePath(path);
}

void EditViewer::setWheel(bool canWheelZoom)
{
    m_bCanWheel = canWheelZoom;
}

qreal EditViewer::getZoomValue() const
{
    return m_ZoomValue;
}

void EditViewer::moveWidgetTool(QWidget* widget)
{
    // shapeTool is top of toolWidget
    //    if (mapToParent(toolWidget->pos()).y() < region.top()) {
    //        //  shapeTool is left or right of toolWidget
    //        if (region.left() + 10 + toolWidget->width() + shapeTool->width() >= screenSize.width()) {
    //            widget->move(QPoint(mapToParent(toolWidget->pos()).x() - widget->width() - 10, region.top() - toolWidget->height() - 10));
    //        } else {
    //            widget->move(QPoint(region.left(), region.top() - toolWidget->height() - widget->height() - 10));
    //        }
    //    }
    //    // shapeTool is left or right of toolWidget
    //    else if (region.bottom() + 10 + widget->height() + toolWidget->height() >= screenSize.height()) {
    //        if (region.left() + 10 + toolWidget->width() + widget->width() >= screenSize.width()) {
    //            widget->move(QPoint(region.left() - widget->width() - 10, region.bottom() + 10));
    //        } else {
    //            widget->move(QPoint(region.left() + 10 + toolWidget->width(), region.bottom() + 10));
    //        }
    //        // shapeTool is bottom of toolWidget
    //    } else if (region.bottom() + 10 + toolWidget->height() <= screenSize.height()) {
    //        widget->move(QPoint(region.left(), region.bottom() + 10 + toolWidget->height()));
    //    }

    widget->move(mapToParent(QPoint(toolWidget->pos().x(), toolWidget->pos().y() + toolWidget->height() + 4)));
}

void EditViewer::moveToolWidget()
{
    //QPoint globalCursorPos = QCursor::pos();
    // int screenNumber = qApp->desktop()->screenNumber(globalCursorPos);
    //QRect sRect = QApplication::desktop()->screen(screenNumber)->geometry();
    toolWidget->move((this->rect().width() - toolWidget->width()) / 2, toolWidget->height() / 4);

    // rigth bottom
    //    if (region.bottom() + 10 + toolWidget->height() >= screenSize.height() && region.left() + toolWidget->width() >= screenSize.width()) {
    //        toolWidget->move(QPoint(toolWidget->x(), region.top() - toolWidget->height() - 10));
    //        // bottom
    //    } else if (region.bottom() + 10 + toolWidget->height() >= screenSize.height()) {
    //        toolWidget->move(QPoint(region.left(), region.top() - toolWidget->height() - 10));
    //        // right
    //    } else if (region.left() + toolWidget->width() >= screenSize.width()) {
    //        toolWidget->move(QPoint(toolWidget->x(), region.bottom() + 10));
    //        // normal
    //    } else {
    //        toolWidget->move(QPoint(region.left(), region.bottom() + 10));
    //    }
}

QRect EditViewer::grabSubRegion()
{
    save = true;
    update();
    int offset = qCore->getBorderWeight();
    QRect r;
    r.setLeft(region.left() + offset);
    r.setTop(region.top() + offset);
    r.setRight(region.right() - offset);
    r.setBottom(region.bottom() - offset);

    qCore->setPixMap(grab(r));
    save = false;
    return r;
}

void EditViewer::resetPaintInfo()
{
    freeLine.clear();
    paintRegion = QRect(-1, -1, 0, 0);
    paintPoint = QPoint(-1, -1);
    paintPoint2 = QPoint(-1, -1);
    paintPoint3 = QPoint(-1, -1);
    eraseRegion = QRect(-1, -1, 0, 0);
    erasePoint = QPoint(-1, -1);
}

int EditViewer::checkInRectBorder(const QPoint& pos)
{
    int borderWeight = qCore->getBorderWeight();
    int x = region.x(), y = region.y();
    int w = region.width(), h = region.height();
    int nx = pos.x(), ny = pos.y();
    if (nx <= x && abs(nx - x) <= borderWeight && (ny <= y + h && ny >= y))
        return LEFT;
    if (nx >= x + w && abs(nx - (x + w)) <= borderWeight && (ny <= y + h && ny >= y))
        return RIGHT;
    if (ny <= y && abs(ny - y) <= borderWeight && (nx <= x + w && nx >= x))
        return TOP;
    if (ny >= y + h && abs(ny - (y + h)) <= borderWeight && (nx <= x + w && nx >= x))
        return BOTTOM;
    if (nx <= x && abs(nx - x) <= borderWeight && ny <= y && abs(ny - y) <= borderWeight)
        return LEFTTOP;
    if (nx <= x && abs(nx - x) <= borderWeight && ny > y + h && abs(ny - (y + h)) <= borderWeight)
        return LEFTBOTTOM;
    if (nx >= x + w && abs(nx - (x + w)) <= borderWeight && ny <= y && abs(ny - y) <= borderWeight)
        return RIGHTTOP;
    if (nx >= x + w && abs(nx - (x + w)) <= borderWeight && ny >= y + h && abs(ny - (y + h)) <= borderWeight)
        return RIGHTBOTTOM;
    if (checkInRectRegion(pos))
        return REGION_IN;
    return 0;
}

bool EditViewer::checkInRectRegion(const QPoint& pos)
{
    return checkEraseInRectRegion(pos, 0);
}

bool EditViewer::checkEraseInRectRegion(const QPoint& pos, int eraseSize)
{
    // pos is the center of erase
    int x = pos.x(), y = pos.y();
    int offset = qCore->getBorderWeight();
    if (x - eraseSize / 2 + offset >= region.left() && y - eraseSize / 2 + offset >= region.top() && x + eraseSize / 2 - offset <= region.right() && y + eraseSize / 2 - offset <= region.bottom())
        return true;
    return false;
}

bool EditViewer::checkValidPoint()
{
    return paintPoint.x() != -1 && paintPoint.y() != -1;
}

void EditViewer::movePixelPanel(const QPoint& pos)
{
    int pw = pixelPanel->width();
    int ph = pixelPanel->height();
    int offset = 20;
    int x = pos.x();
    int y = pos.y();

    pixelPanel->move(x + offset, y + offset);

    if (ph + y + offset >= screenSize.height() && pw + x + offset >= screenSize.width()) {
        pixelPanel->move(x - pw - offset, y - offset - ph);
    } else if (ph + y + offset >= screenSize.height()) {
        pixelPanel->move(x + offset, y - offset - ph);
    } else if (pw + x + offset >= screenSize.width()) {
        pixelPanel->move(x - pw - offset, y + offset);
    } else {
        pixelPanel->move(x + offset, y + offset);
    }
    QPoint image_pos;
    if (m_bWndView) {
        image_pos.setX(x - m_imageX);
        image_pos.setY(y - m_imageY);
    } else {
        image_pos.setX(x);
        image_pos.setY(y);
    }

    pixelPanel->setPos(image_pos);
    pixelPanel->show();
    pixelPanel->update();
}

bool EditViewer::checkValidPaintRegion()
{
    return paintRegion.left() != -1 && paintRegion.top() != -1 && paintRegion.width() && paintRegion.height();
}

void EditViewer::resizeRegion(const QPoint& fromPos, const QPoint& toPos, QRect& r)
{
    int x1 = fromPos.x();
    int y1 = fromPos.y();
    int x2 = toPos.x();
    int y2 = toPos.y();
    if (x2 > x1 && y2 > y1) {
        r.setRect(x1, y1, x2 - x1, y2 - y1);
    } else if (x2 > x1 && y2 < y1) {
        r.setRect(x1, y2, x2 - x1, y1 - y2);
    } else if (x2 < x1 && y2 < y1) {
        r.setRect(x2, y2, x1 - x2, y1 - y2);
    } else if (x2 < x1 && y2 > y1) {
        r.setRect(x2, y1, x1 - x2, y2 - y1);
    }
}

void EditViewer::drawDots(QPainter& p)
{
    if (region.width() <= 0 && region.height() <= 0)
        return;
    int x = region.x();
    int y = region.y();
    int w = region.width();
    int h = region.height();
    int size = qCore->getDotSize();
    auto c = qCore->getDotColor();

    p.fillRect(x - size / 2, y - size / 2, size, size, c);
    p.fillRect(x - size / 2, y + h / 2 - size / 2, size, size, c);
    p.fillRect(x - size / 2, y + h - size / 2, size, size, c);
    p.fillRect(x + w / 2 - size / 2, y - size / 2, size, size, c);
    p.fillRect(x + w / 2 - size / 2, y + h - size / 2, size, size, c);
    p.fillRect(x + w - size / 2, y - size / 2, size, size, c);
    p.fillRect(x + w - size / 2, y + h / 2 - size / 2, size, size, c);
    p.fillRect(x + w - size / 2, y + h - size / 2, size, size, c);
}

void EditViewer::drawTopPosTip(QPainter& p)
{
    p.setPen(QPen(settings->colorScheme().text, 2));
    QString txt = QString("%1 x %2 px").arg(region.width()).arg(region.height());
    int fh = p.fontMetrics().height();
    int fw = p.fontMetrics().horizontalAdvance(txt);

    int padding = 2;
    int x = region.left();
    int y = region.top() - (fh + padding * 4 + 5);

    // up
    if (y <= 0)
        x = region.right() + 10, y = region.top();
    // leftbottom
    if (region.bottom() + 10 + toolWidget->height() >= screenSize.height())
        x = region.right() + 10, y = region.top();
    // righttop/rightbottom
    if (x + (fw + padding * 4) >= screenSize.width())
        x = region.left() - (fw + padding * 4) - 10;

    p.drawRoundedRect(x, y, (fw + padding * 4), fh + padding * 2, 3, 3);
    p.drawText(x + padding * 2, y + fh / 2 + padding * 2 + 5, txt);
}

bool EditViewer::pinWidgetVisiable()
{
    return toolWidget->pinWidgetVisiable;
}

void EditViewer::moveGrabWindow(const QPoint& pos)
{
    //     int w = region.width(), h = region.height();
    //     QRect r;
    //     r.setX(pos.x() - offset.x());
    //     r.setY(pos.y() - offset.y());
    //     r.setWidth(w);
    //     r.setHeight(h);
    //     if (r.left() <= 0 && r.top() <= 0)
    //         region = QRect(0, 0, r.width(), r.height());
    //     else if (r.left() <= 0 && r.bottom() >= screenSize.height())
    //         region = QRect(0, screenSize.height() - r.height(), r.width(), r.height());
    //     else if (r.right() >= screenSize.width() && r.bottom() >= screenSize.height())
    //         region = QRect(screenSize.width() - r.width(), screenSize.height() - r.height(), r.width(), r.height());
    //     else if (r.right() >= screenSize.width() && r.top() <= 0)
    //         region = QRect(screenSize.width() - r.width(), 0, r.width(), r.height());
    //     else if (r.left() <= 0)
    //         region = QRect(0, r.y(), r.width(), r.height());
    //     else if (r.top() <= 0)
    //         region = QRect(r.x(), 0, r.width(), r.height());
    //     else if (r.bottom() >= screenSize.height())
    //         region = QRect(r.x(), screenSize.height() - r.height(), r.width(), r.height());
    //     else if (r.right() >= screenSize.width())
    //         region = QRect(screenSize.width() - r.width(), r.y(), r.width(), r.height());
    //     else
    //         region = r;
}

void EditViewer::paintEvent(QPaintEvent* event)
{
    // 绘制样式
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    p.fillRect(this->rect(), settings->colorScheme().background);  //QColor最后一个参数80代表背景的透明度
    //style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    if (m_Image.isNull()) {
        p.drawImage(this->rect(), m_Image);
        return QWidget::paintEvent(event);
    }

    // 设置绘制区域
    int painter_w = m_Image.width() * m_ZoomValue;
    int painter_h = m_Image.height() * m_ZoomValue;

    int painter_x = (rect().left() + rect().right()) / 2 - painter_w / 2 + m_XPtInterval;
    int topOffset = toolWidget->height() + toolWidget->height() / 2 + 10;
    int bottomOffset = 10;
    int painter_y = (rect().top() + topOffset + rect().bottom() - bottomOffset) / 2 - painter_h / 2 + m_YPtInterval;

    //    p.translate(painter_x, painter_y);
    QRect picRect(painter_x, painter_y, painter_w, painter_h);
    m_imageX = painter_x;
    m_imageY = painter_y;
    if (!m_bEdit) {
        p.drawImage(picRect, m_Image);
    }

    if (!m_bEdit)
        return;

    if (!completed) {
        QScreen* currScreen = QApplication::screenAt(QCursor::pos());
        if (m_bWndView) {
            qCore->setBgPixmap(QPixmap::fromImage(m_Image));
        } else {
            qCore->setBgPixmap(currScreen->grabWindow(0));
            QPalette palatte = palette();
            palatte.setBrush(QPalette::Window, qCore->getbgPixmap());
            setPalette(palatte);
        }

        completed = true;
    }
    moveToolWidget();

    if (pressed || completed || cutTopWindow) {
        region = picRect;
        p.eraseRect(region);
        if (m_bWndView) {
            p.drawPixmap(region, qCore->getbgPixmap());
        }

        // draw shape_object
        if (paintEventNotify == PaintEventNotify::Shape || paintEventNotify == PaintEventNotify::Pen
            || paintEventNotify == PaintEventNotify::Erase || paintEventNotify == PaintEventNotify::Text
            || paintEventNotify == PaintEventNotify::Arrow || paintEventNotify == PaintEventNotify::Blur
            || paintEventNotify == PaintEventNotify::Brush) {

            p.setClipRect(region);
            p.eraseRect(rect());
            if (m_bWndView) {
                p.drawPixmap(region, qCore->getbgPixmap());
            }
            QPixmap tmpBgPixmap = qCore->getbgPixmap();
            QPainter pt(&tmpBgPixmap);
            int size = sos.size(), i = 0;
            if (size) {
                do {
                    // set pen for per shape_object
                    p.setPen(QPen(sos[i].color, sos[i].pensize));
                    pt.setPen(p.pen());

                    switch (sos[i].st) {
                    case ShapeType::Rectangle:
                    case ShapeType::Circle:
                    case ShapeType::Triangle:
                    case ShapeType::RTriangle:
                    case ShapeType::Brush: {
                        ShapeDrawer(sos[i], p, this);
                        ShapeDrawer(sos[i], pt, this);
                        break;
                    }
                    case ShapeType::Point:
                    case ShapeType::Line:
                    case ShapeType::DashLine:
                    case ShapeType::Curve: {
                        p.setPen(QPen(sos[i].color, sos[i].pensize));
                        pt.setPen(QPen(sos[i].color, sos[i].pensize));
                        PenDrawer(sos[i], p, this);
                        PenDrawer(sos[i], pt, this);
                        break;
                    }
                    case ShapeType::TriArrow:
                    case ShapeType::LineArrow: {
                        ArrowDrawer(sos[i].ps.front(), sos[i].ps.back(), 5 * sos[i].pensize, sos[i].color, p, sos[i].st);
                        ArrowDrawer(sos[i].ps.front(), sos[i].ps.back(), 5 * sos[i].pensize, sos[i].color, pt, sos[i].st);
                        break;
                    }
                    case ShapeType::Text: {
                        TextDrawer(sos[i], p, region, this);
                        TextDrawer(sos[i], pt, region, this);
                        break;
                    }
                    case ShapeType::Blur: {
                        RectangleBlurDrawer(tmpBgPixmap, sos[i].rs.first(), region, p, qCore->blurRadius(), this);
                        RectangleBlurDrawer(tmpBgPixmap, sos[i].rs.first(), region, pt, qCore->blurRadius(), this);
                        break;
                    }
                    case ShapeType::Erase: {
                        EraseDrawer(sos[i], p, pt,region,this);
                        break;
                    }
                    default:
                        break;
                    }
                    i++;
                } while (i < size);
            }
            // preview
            {
                QColor bgcolor = qCore->getBgColor();
                p.setPen(QPen(qCore->getPenColor(), qCore->getPenSize()));
                switch (shapeType) {
                case ShapeType::Rectangle:
                case ShapeType::Circle:
                case ShapeType::Brush: {
                    if (checkValidPaintRegion()) {
                        if (fill) {
                            p.setBrush(bgcolor);
                        }
                        if (qCore->getPenSize()) {
                            if (shapeType == ShapeType::Rectangle) {
                                p.drawRect(paintRegion);
                            } else if (shapeType == ShapeType::Circle) {
                                p.drawEllipse(paintRegion);
                            } else if (shapeType == ShapeType::Brush) {
                                QColor c = qCore->getPenColor();
                                c = brushTool->color();
                                p.fillRect(paintRegion, QColor(c.red(), c.green(), c.blue(), qCore->brushOpacity()));
                            }
                        }
                        p.setBrush(Qt::NoBrush);
                    }
                    break;
                }
                case ShapeType::Blur: {
                    if (checkValidPaintRegion()) {
                        RectangleBlurDrawer(tmpBgPixmap, paintRegion, region, p, qCore->blurRadius(), this);
                    }
                    break;
                }
                case ShapeType::Line:
                case ShapeType::DashLine: {
                    if (checkValidPoint() && paintPoint2.x() != -1 && paintPoint2.y() != -1) {
                        if (shapeType == ShapeType::DashLine) {
                            p.setPen(QPen(qCore->getPenColor(), qCore->getPenSize(), Qt::DashLine));
                        }
                        p.drawLine(paintPoint, paintPoint2);
                    }
                    break;
                }
                case ShapeType::TriArrow:
                case ShapeType::LineArrow: {
                    p.setPen(QPen(arrowTool->color(), arrowTool->penSize()));
                    ArrowDrawer(paintPoint, paintPoint2, 5 * qCore->getPenSize(), qCore->getPenColor(), p, shapeType);
                    break;
                }
                case ShapeType::Curve: {
                    if (freeLine.size() <= 0)
                        break;
                    QPainterPath pp;
                    for (std::size_t j = 0; j + 1 < freeLine.size(); j++) {
                        pp.moveTo(freeLine[j]);
                        pp.lineTo(freeLine[j + 1]);
                    }
                    p.setPen(QPen(penTool->color(), penTool->penSize()));
                    p.drawPath(pp);
                    break;
                }
                case ShapeType::Triangle:
                case ShapeType::RTriangle: {
                    if (checkValidPoint() && paintPoint2.x() != -1 && paintPoint2.y() != -1) {
                        QVector<QPoint> x = { paintPoint, paintPoint2, paintPoint3, paintPoint };
                        if (paintPoint2.x() < region.x()) {
                            paintPoint2.setX(region.x());
                            paintPoint3.setX((paintPoint.x() - paintPoint2.x()));
                        }
                        QPainterPath pp;
                        pp.addPolygon(QPolygonF(x));
                        if (fill) {
                            p.fillPath(pp, bgcolor);
                        }
                        p.drawPath(pp);
                        p.setBrush(Qt::NoBrush);
                    }
                    break;
                }
                case ShapeType::Point: {
                    if (checkValidPoint()) {
                        p.drawPoint(paintPoint);
                    }
                    break;
                }
                case ShapeType::Erase: {
                    if (egeneral) {
                        for (auto&& x : freeLine) {
                            QRect r(x.x() - eraseSize / 2, x.y() - eraseSize / 2, eraseSize, eraseSize);
                            p.eraseRect(r);
                        }
                    } else {
                        if (erasePoint.x() != -1 && erasePoint.y() != -1 && eraseRegion.x() != -1 && eraseRegion.y() != -1)
                            p.eraseRect(eraseRegion.normalized().adjusted(-2, -2, 2, 2));
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
        // draw region border
        p.setPen(QPen(qCore->getBorderColor(), qCore->getBorderWeight()));
        p.drawRect(region);

        //         if (!save)
        //             drawDots(p);

        if (region.width() && region.height())
            drawTopPosTip(p);
    }
    QWidget::paintEvent(event);
}

void EditViewer::wheelEvent(QWheelEvent* event)
{
    if (m_bEdit) {
        QWidget::wheelEvent(event);
        return;
    }
    int value = event->delta();
    if (m_bCanWheel) {
        if (value > 0) {
            onZoomInImage();
        } else {
            onZoomOutImage();
        }
    } else {
        sigWheel(value);
    }

    this->update();
    QWidget::wheelEvent(event);
}

void EditViewer::mousePressEvent(QMouseEvent* event)
{
    //显示的图片宽度或者高度超过窗口的时候
    m_OldPos = event->pos();
    if (!m_bEdit) {
        event->ignore();
        return;
    }

    if (event->button() == Qt::LeftButton) {
        cutTopWindow = false;
        if (!completed) {
            setCursor(Qt::PointingHandCursor);
            point = event->pos();
            pixelPanel->move(event->pos());
        }
        d = checkInRectBorder(event->pos());

        switch (d) {
        case LEFT:
        case RIGHT:
            setCursor(Qt::ArrowCursor); // (Qt::SizeHorCursor);
            painting = false;
            break;
        case TOP:
        case BOTTOM:
            setCursor(Qt::ArrowCursor); //(Qt::SizeVerCursor);
            painting = false;
            break;
        case LEFTTOP:
        case RIGHTBOTTOM:
            setCursor(Qt::ArrowCursor); //(Qt::SizeFDiagCursor);
            painting = false;
            break;
        case RIGHTTOP:
        case LEFTBOTTOM:
            setCursor(Qt::ArrowCursor); //(Qt::SizeBDiagCursor);
            painting = false;
            break;
        case REGION_IN: {
            freeLine.clear();

            if (paintEventNotify == PaintEventNotify::Shape || paintEventNotify == PaintEventNotify::Pen
                || paintEventNotify == PaintEventNotify::Erase || paintEventNotify == PaintEventNotify::Text
                || paintEventNotify == PaintEventNotify::Arrow || paintEventNotify == PaintEventNotify::Blur
                || paintEventNotify == PaintEventNotify::Brush) {

                painting = true;
                bool erase_ok = true;
                switch (shapeType) {
                case ShapeType::Rectangle:
                case ShapeType::Circle:
                case ShapeType::Triangle:
                case ShapeType::RTriangle:
                case ShapeType::Line:
                case ShapeType::DashLine:
                case ShapeType::Curve:
                case ShapeType::Point:
                case ShapeType::TriArrow:
                case ShapeType::LineArrow:
                case ShapeType::Blur:
                case ShapeType::Brush:
                    setCursor(Qt::CrossCursor);
                    break;

                case ShapeType::Text: {
                    setCursor(Qt::IBeamCursor);
                    m_edit->draw();
                    QSize size(100, 50);
                    m_edit->setFocus();
                    auto pos = QPoint(event->pos().x(), event->y());
                    if (pos.x() + size.width() > region.x() + region.width())
                        pos.setX(region.x() + region.width() - size.width());
					if (pos.y() + size.height() > region.y() + region.height())
						pos.setY(region.y() + region.height() - size.height());
                    m_edit->move(pos);
                    m_edit->show();
                    m_edit->clear();
                    break;
                }
                case ShapeType::Erase: {
                    // check erase is in region
                    if (checkEraseInRectRegion(event->pos(), eraseSize)) {
                        if (egeneral) {
                            setCursor(Qt::ForbiddenCursor);
                            eraseRegion.setRect(event->pos().x() / eraseSize / 2, event->pos().y() + eraseSize / 2, eraseSize, eraseSize);
                        } else {
                            setCursor(Qt::CrossCursor);
                            erasePoint = event->pos();
                            eraseRegion.setX(erasePoint.x());
                            eraseRegion.setY(erasePoint.y());
                        }
                    } else
                        erase_ok = false;
                    break;
                }
                default:
                    setCursor(Qt::CrossCursor);
                    break;
                }
                if (erase_ok) {
                    // modify paint region
                    paintPoint = event->pos();
                    paintRegion = QRect(paintPoint.x(), paintPoint.y(), 0, 0);
                    freeLine.push_back(paintPoint);
                }
            } else {
                setCursor(Qt::ArrowCursor);
                offset.setX(event->pos().x() - region.left());
                offset.setY(event->pos().y() - region.top());
            }
            break;
        }
        default: {
            setCursor(Qt::ArrowCursor);
        }
        }
        pressed = true;
    }
}

void EditViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bEdit) {
        if (!(event->buttons() & Qt::LeftButton))
            return QWidget::mouseMoveEvent(event);

        this->setCursor(Qt::CrossCursor); // SizeAllCursor);
        QPoint pos = event->pos();
        int xPtInterval = pos.x() - m_OldPos.x();
        int yPtInterval = pos.y() - m_OldPos.y();
        if ((m_ZoomValue * m_Image.width()) > this->width()) {
            m_XPtInterval += xPtInterval;
        }

        if ((m_ZoomValue * m_Image.height()) > this->height()) {
            m_YPtInterval += yPtInterval;
        }

        m_OldPos = pos;
        this->update();
        event->ignore();
        return;
    }

    if (completed) {
        if (!pressed) {
            switch (checkInRectBorder(event->pos())) {
            case LEFT:
            case RIGHT:
                setCursor(Qt::ArrowCursor); // Qt::SizeHorCursor);
                break;
            case TOP:
            case BOTTOM:
                setCursor(Qt::ArrowCursor); //(Qt::SizeVerCursor);
                break;
            case LEFTTOP:
            case RIGHTBOTTOM:
                setCursor(Qt::ArrowCursor); //(Qt::SizeFDiagCursor);
                break;
            case RIGHTTOP:
            case LEFTBOTTOM:
                setCursor(Qt::ArrowCursor); //(Qt::SizeBDiagCursor);
                break;
            case REGION_IN: {
                if (!painting && resizing) {
                    // resize the region window
                    cursorPos = event->pos();
                    movePixelPanel(event->pos());
                    setCursor(Qt::ArrowCursor);
                } else {
                    switch (shapeType) {
                    case ShapeType::Text:
                        setCursor(Qt::IBeamCursor);
                        break;
                    case ShapeType::Line:
                    case ShapeType::Point:
                    case ShapeType::LineArrow:
                    case ShapeType::Curve:
                    case ShapeType::Rectangle:
                    case ShapeType::Brush:
                    case ShapeType::TriArrow:
                    case ShapeType::DashLine:
                    case ShapeType::RTriangle:
                    case ShapeType::Blur:
                    case ShapeType::Circle:
                    case ShapeType::Triangle:
                        setCursor(Qt::CrossCursor);
                        break;
                    case ShapeType::Erase: {
                        if (egeneral)
                            setCursor(Qt::ForbiddenCursor);
                        else
                            setCursor(Qt::CrossCursor);
                        break;
                    }
                    default: {
                        setCursor(Qt::ArrowCursor);
                        break;
                    }
                    }
                }
                break;
            }
            default: {
                setCursor(Qt::ArrowCursor);
                pixelPanel->hide();
            }
            }
        }
        // left button pressed
        if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton) {
            moveToolWidget();

            eraseTool->hide();
            pixelPanel->hide();
            shapeTool->hide();
            penTool->hide();
            textTool->hide();
            brushTool->hide();
            arrowTool->hide();
            //d = checkInRectBorder(event->pos());
            if (d == LEFT) {
                //                 if (event->pos().x() <= region.right())
                //                     region.setX(event->pos().x());
                //             } else if (d == TOP) {
                //                 if (event->pos().y() <= region.bottom())
                //                     region.setY(event->pos().y());
                //             } else if (d == RIGHT) {
                //                 if (event->pos().x() >= region.left())
                //                     region.setRight(event->pos().x());
                //             } else if (d == BOTTOM) {
                //                 if (event->pos().y() >= region.top())
                //                     region.setBottom(event->pos().y());
                //             } else if (d == LEFTTOP) {
                //                 if (event->pos().x() <= region.bottomRight().x() && event->pos().y() <= region.bottomRight().y())
                //                     region.setTopLeft(event->pos());
                //             } else if (d == RIGHTTOP) {
                //                 if (event->pos().x() >= region.bottomLeft().x() && event->pos().y() <= region.bottomLeft().y())
                //                     region.setTopRight(event->pos());
                //             } else if (d == LEFTBOTTOM) {
                //                 if (event->pos().x() <= region.topRight().x() && event->pos().y() >= region.topRight().y())
                //                     region.setBottomLeft(event->pos());
                //             } else if (d == RIGHTBOTTOM) {
                //                 if (event->pos().x() >= region.topLeft().x() && event->pos().y() >= region.topLeft().y())
                //                     region.setBottomRight(event->pos());
            } else if (d == REGION_IN) {

                // paint region
                if (paintEventNotify == PaintEventNotify::Shape || paintEventNotify == PaintEventNotify::Pen
                    || paintEventNotify == PaintEventNotify::Erase || paintEventNotify == PaintEventNotify::Text
                    || paintEventNotify == PaintEventNotify::Arrow || paintEventNotify == PaintEventNotify::Blur
                    || paintEventNotify == PaintEventNotify::Brush) {

                    auto r = QRect(paintPoint, event->pos()).intersected(region);
                    auto endPoint = (r.center() - paintPoint) * 2 + paintPoint;

                    if (shapeType == ShapeType::Brush || shapeType == ShapeType::Rectangle || shapeType == ShapeType::Circle || shapeType == ShapeType::Blur) {
                        paintRegion.setWidth(endPoint.x() - paintPoint.x());
                        paintRegion.setHeight(endPoint.y() - paintPoint.y());
                    } else if (shapeType == ShapeType::DashLine || shapeType == ShapeType::Line
                        || shapeType == ShapeType::LineArrow || shapeType == ShapeType::TriArrow) {
                        paintPoint2 = endPoint;
                    } else if (shapeType == ShapeType::Curve) {
                        freeLine.push_back(endPoint);
                    } else if (shapeType == ShapeType::RTriangle || shapeType == ShapeType::Triangle) {
                        // paintPoint2 is left
                        // paintPoint3 is right
                        paintPoint2.setY(endPoint.y());
                        paintPoint3.setY(endPoint.y());
                        if (shapeType == ShapeType::RTriangle)
                            paintPoint2.setX(paintPoint.x());
                        else {
                            paintPoint2.setX(2 * paintPoint.x() - endPoint.x());
                            if (paintPoint2.x() < region.x())
                                paintPoint2.setX(region.x());
                            if (paintPoint2.x() > region.right())
                                paintPoint2.setX(region.right());
                        }
                        paintPoint3.setX(endPoint.x());
                    } else if (shapeType == ShapeType::Erase) {
                        eraseRegion.setWidth(endPoint.x() - erasePoint.x());
                        eraseRegion.setHeight(endPoint.y() - erasePoint.y());
                        if (checkEraseInRectRegion(endPoint, eraseSize)) {
                            if (egeneral) {
                                freeLine.push_back(endPoint);
                            }
                        }
                    }
                } else {
                    //moveGrabWindow(event->pos());
                }
            }
            update();
        }
        return;
    }
    //#ifdef Q_OS_WIN
    //    if(!pressed) {
    //        for(auto x=rects.begin();x!=rects.end();x++){
    //            auto r=QRect(*x);
    //            if(r.contains(event->pos())){
    //                region=r;
    //                break;
    //            }
    //        }
    //        cutTopWindow=true;
    //        update();
    //        return;
    //    }
    //#endif
    setCursor(Qt::CrossCursor);

    movePixelPanel(event->pos());

    //resizeRegion(point, event->pos(), region);
    update();
}

void EditViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_bEdit) {
        this->setCursor(Qt::ArrowCursor);
        event->ignore();
        return;
    }

    if (event->button() == Qt::LeftButton) {

        if (region.width() == 0 || region.height() == 0)
            return;
        pixelPanel->hide();

        // grab window pixmap to pixmap object
        grabSubRegion();

        // resize region window
        if (!painting && resizing)
            setCursor(Qt::OpenHandCursor);

        pressed = false;
        completed = true;

        if (paintEventNotify == PaintEventNotify::Shape || paintEventNotify == PaintEventNotify::Pen
            || paintEventNotify == PaintEventNotify::Erase || paintEventNotify == PaintEventNotify::Text
            || paintEventNotify == PaintEventNotify::Arrow || paintEventNotify == PaintEventNotify::Blur
            || paintEventNotify == PaintEventNotify::Brush) {
            if (!painting)
                return;
            auto r = QRect(paintPoint, event->pos()).intersected(region);
            auto endPoint = (r.center() - paintPoint) * 2 + paintPoint;
            resizeRegion(paintPoint, endPoint, paintRegion);
            resizeRegion(erasePoint, endPoint, eraseRegion);
            shape_object so;
            so.color = qCore->getPenColor();
            so.pensize = qCore->getPenSize();
            so.bgcolor = qCore->getBgColor();
            so.fill = fill;
            so.isblur = false;

            if (paintEventNotify == PaintEventNotify::Arrow ) {
                so.pensize = arrowTool->penSize();
                so.color = arrowTool->color();
            }
            if (paintEventNotify == PaintEventNotify::Brush) {
                so.color = brushTool->color();
            }
            if (paintEventNotify == PaintEventNotify::Pen) {
				so.pensize = penTool->penSize();
				so.color = penTool->color();
            }

            if (!checkValidPoint())
                goto invalid;

            if (shapeType != ShapeType::Point && shapeType != ShapeType::Line
                && shapeType != ShapeType::TriArrow && shapeType != ShapeType::DashLine
                && shapeType != ShapeType::Point && shapeType != ShapeType::LineArrow && !checkValidPaintRegion()) {
                goto invalid;
            }

            if (shapeType == ShapeType::Brush || shapeType == ShapeType::Blur || shapeType == ShapeType::Rectangle || shapeType == ShapeType::Circle) {
                // check is a rectangle
                if (paintRegion.width() == 0 || paintRegion.height() == 0)
                    return;
                so.st = shapeType;
                if (shapeType == ShapeType::Blur) {
                    if (paintRegion.x() <= region.x())
                        paintRegion.setX(region.x());
                    if (paintRegion.y() <= region.y())
                        paintRegion.setY(region.y());
                    if (paintRegion.right() >= region.right())
                        paintRegion.setRight(region.right());
                    if (paintRegion.bottom() >= region.bottom())
                        paintRegion.setBottom(region.bottom());
                }
                so.rs.push_back(paintRegion);
                sos.push_back(so);
            } else if (shapeType == ShapeType::DashLine || shapeType == ShapeType::Line
                || shapeType == ShapeType::TriArrow || shapeType == ShapeType::LineArrow) {
                if (checkValidPoint() && paintPoint2.x() != -1 && paintPoint2.y() != -1) {
                    so.st = shapeType;
                    so.ps.push_back(paintPoint);
                    so.ps.push_back(paintPoint2);
                    sos.push_back(so);
                }
            } else if (shapeType == ShapeType::Point) {
                so.st = shapeType;
                so.ps.push_back(paintPoint);
                sos.push_back(so);
            } else if (shapeType == ShapeType::Curve) {
                so.st = shapeType;
                for (auto pos : freeLine)
                    so.ps.push_back(pos);
                sos.push_back(so);

            } else if (shapeType == ShapeType::RTriangle || shapeType == ShapeType::Triangle) {
                if (checkValidPoint() && paintPoint2.x() != -1 && paintPoint2.y() != -1) {
                    so.st = shapeType;
                    so.ps.push_back(paintPoint);
                    so.ps.push_back(paintPoint2);
                    so.ps.push_back(paintPoint3);
                    so.ps.push_back(paintPoint);
                    sos.push_back(so);
                }
            } else if (shapeType == ShapeType::Erase) {
                // not empty shape_objects
                if (sos.size() > 0) {
                    so.st = shapeType;
                    so.egeneral = egeneral;
                    if (egeneral) {
                        so.erasesize = eraseSize;
                        for (auto&& x : freeLine)
                            so.ps.push_back(x);
                        sos.push_back(so);
                    } else {
                        if (eraseRegion.x() <= region.x())
                            eraseRegion.setX(region.x());
                        if (eraseRegion.y() <= region.y())
                            eraseRegion.setY(region.y());
                        if (eraseRegion.right() >= region.right())
                            eraseRegion.setRight(region.right());
                        if (eraseRegion.bottom() >= region.bottom())
                            eraseRegion.setBottom(region.bottom());

                        so.rs.push_back(eraseRegion);
                        if (eraseRegion.x() != -1 && eraseRegion.y() != -1)
                            sos.push_back(so);
                    }
                }
            }
        }

        moveToolWidget();
        toolWidget->show();

    invalid:
        resetPaintInfo();
    }
}

void EditViewer::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    setZoomValue();
}

void EditViewer::setZoomValue()
{
    int picWidth = m_Image.size().width();
    int picHeight = m_Image.size().height();
    int offset = toolWidget->height() + toolWidget->height() / 2 + 20;
    int winWidth = this->width();
    int winHeight = this->height() - offset;
    qreal picRatio = 1.0 * picWidth / picHeight;
    qreal screenRatio = 1.0 * winWidth / winHeight;

    if (picWidth <= winWidth && picHeight <= winHeight) {
        //宽和高都小于屏幕尺寸时，原样显示
        m_ZoomValue = 1.0;
    } else if (picHeight > winHeight && picWidth <= winWidth) {
        //高度大于屏幕并且宽度小于屏幕时， 按高度缩放
        m_ZoomValue = 1.0 * winHeight / picHeight;
    } else if (picWidth > winWidth && picHeight <= winHeight) {
        //宽度大于屏幕并且高度小于屏幕时， 按宽度缩放
        m_ZoomValue = 1.0 * winWidth / picWidth;
    } else if (picWidth > winWidth && picHeight > winHeight) {
        if (picRatio <= screenRatio) {
            //宽度和高度都大于屏幕并且图片宽高比小于屏幕时，按高度缩放
            m_ZoomValue = 1.0 * winHeight / picHeight;
        } else {
            //宽度和高度都大于屏幕并且图片宽高比大于屏幕时，按宽度缩放
            m_ZoomValue = 1.0 * winWidth / picWidth;
        }
    }
}

void EditViewer::onZoomInImage(double delta)
{
    if (m_bEdit)
        return;
    if (m_ZoomValue >= 50)
        return;
    m_ZoomValue += delta;
    completed = false;
    this->update();
}

void EditViewer::onZoomOutImage(double delta)
{
    if (m_bEdit)
        return;
    m_ZoomValue -= delta;
    if (m_ZoomValue <= delta) {
        m_ZoomValue += delta;
        return;
    }
    completed = false;
    this->update();
}

void EditViewer::onPresetImage()
{
    m_ZoomValue = 1.0;
    this->update();
}

void EditViewer::setViewMode(bool bIsWindowView)
{
    m_bWndView = bIsWindowView;
//    if (m_bWndView)
//    {
//        toolWidget = m_toolWidget2;
//    }
}

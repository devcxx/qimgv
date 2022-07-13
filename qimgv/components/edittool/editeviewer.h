#ifndef EditViewer_H
#define EditViewer_H

#include <QWidget>

#include "core/edit_core.h"
#include "core/core_system.h"
#include "widget/toolwidget.h"
#include "widget/inputtextedit.h"
#include "widget/pixelpanel.h"
#include "tool/texttool.h"
#include "tool/shapetool.h"
#include "tool/pentool.h"
#include "tool/erasetool.h"
#include "tool/arrowtool.h"

class  EditViewer : public QWidget {
    Q_OBJECT

    enum {
        LEFT=1,
        RIGHT,
        TOP,
        BOTTOM,
        LEFTTOP,
        RIGHTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        REGION_IN
    };

public:
    enum class Mode {
        Mode_Readonly,
        Mode_ReadWrite
    };

    EditViewer(QWidget* parent = nullptr);
    virtual ~EditViewer();
    void init();
    void initShortcuts();
    void setEditMode(bool editMode, Mode mode = Mode::Mode_Readonly);
    void setImage(const QImage& image);
    void setImagePath(const QString& path);
    void setWheel(bool canWheelZoom = false);
    qreal getZoomValue() const;
    void moveWidgetTool(QWidget *);
    void moveToolWidget();
    QRect grabSubRegion();
    void resetPaintInfo();
    int checkInRectBorder(const QPoint &);
    bool checkInRectRegion(const QPoint &);
    bool checkEraseInRectRegion(const QPoint &, int);
    bool checkValidPoint();
    void movePixelPanel(const QPoint&);
    bool checkValidPaintRegion();
    void resizeRegion(const QPoint&,const QPoint&,QRect &);
    void drawDots(QPainter &);
    void drawTopPosTip(QPainter& p);
    void moveGrabWindow(const QPoint &);
    bool pinWidgetVisiable();
    void setViewMode(bool bIsWindowView);

signals:
    void sigWheel(int);
    void editCancel();
    void editDone();

public slots:
    void cancel();
    void copyColorValue();
    void changeRgb2Hex();

    void upMove();
    void downMove();
    void leftMove();
    void rightMove();

    void upMovePixelPanel();
    void downMovePixelPanel();
    void leftMovePixelPanel();
    void rightMovePixelPanel();

    void onZoomInImage(double delta = 0.2); //放大
    void onZoomOutImage(double delta = 0.2); //缩小
    void onPresetImage(void); //重置

protected:
    //void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event);

    void setZoomValue();

private:
    QString m_imgPath;
    QImage m_Image;
    qreal m_ZoomValue;
    int m_XPtInterval;
    int m_YPtInterval;
    QPoint m_OldPos;
    bool m_bCanWheel; //是否支持滚轮放大缩小
    bool m_bEdit;
    Mode m_editMode;
    bool m_bWndView = true; // true：窗口模式查看图片，false：非窗口模式
    int m_imageX;
    int m_imageY;

private:
    QRect region;
    QPoint point;
    QPoint offset;

    int d, eraseSize;
    bool pressed;
    bool completed;
    bool painting;
    bool fill;
    bool save;
    bool resizing;
    bool egeneral;

    bool cutTopWindow=false;
    ToolWidget *toolWidget;
//    ToolWidget* m_toolWidget1;
//    ToolWidget* m_toolWidget2;
    ShapeTool *shapeTool;
    InputTextEdit *inputTextEdit;
    TextEdit* m_edit;
    PixelPanel *pixelPanel;
    PenTool *penTool;
    TextTool *textTool;
    EraseTool *eraseTool;
    ArrowTool *arrowTool;
    BrushTool* brushTool;

    PaintEventNotify paintEventNotify;
    ShapeType shapeType;

    QRect paintRegion;
    QRect eraseRegion;
    QPoint paintPoint,paintPoint2,paintPoint3,erasePoint;
    QPoint cursorPos;
    QSize screenSize;

    vector<shape_object> sos;
    vector<QPoint>freeLine;

    QVector<QRect> rects;

    friend class ToolWidget;
    friend class ShapeTool;
    friend class PenTool;
    friend class PixelPanel;
    friend class InputTextEdit;
    friend class TextTool;
    friend class EraseTool;
    friend class ArrowTool;
    friend class TextEdit;
};

#endif // EditViewer_H

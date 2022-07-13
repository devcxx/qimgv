#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include "../tool/basetool.h"
#include <stack>

using std::stack;

class EditViewer;
class BaseButton;
class PinWidget;
class ToolWidget : public BaseTool
{
    Q_OBJECT

public:
    explicit ToolWidget(QWidget *parent = nullptr);
    ~ToolWidget();
    void singlePressed();
    void paintEvent(QPaintEvent *event);
    void initUI();
    
    void triggerInputTextEdit();
    void setViewMode(bool bIsWindowView);
protected slots:
    void on_btnSave2File_clicked();
    void on_btnCopyClipboard_clicked();
    void on_btnCancel_clicked();
    void on_btnShape_clicked();
    void on_btnPen_clicked();
    void on_btnArrow_clicked();
    void on_btnText_clicked();
    void on_btnErase_clicked();
    void on_btnUndo_clicked();
    void on_btnRedo_clicked();
    void on_btnBlur_clicked();
    void on_btnBrush_clicked();
    void on_btnPin_clicked();
public:
    bool pinWidgetVisiable = false;
private:
    PinWidget * pinWidget;
    EditViewer * captureW;
    stack<shape_object> restore;

    BaseButton *btnShape;
    BaseButton *btnPen;
    BaseButton *btnText;
    BaseButton *btnArrow;
    BaseButton *btnErase;
    BaseButton *btnBlur;
    BaseButton *btnBrush;
    BaseButton *btnUndo;
    BaseButton *btnRedo;
    BaseButton *btnPin;
    BaseButton *btnCancel;
    BaseButton *btnSave;
    BaseButton *btnCopy;
};

#endif

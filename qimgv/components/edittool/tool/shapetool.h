#ifndef SHAPETOOL_H
#define SHAPETOOL_H

#include "basetool.h"

class EditViewer;
class BaseButton;

class ShapeTool : public BaseTool
{
    Q_OBJECT

public:
    explicit ShapeTool(QWidget *parent = nullptr);
    ~ShapeTool();
    void singlePressed();
    void initUI();

public slots:
    void on_btnRectangle_clicked();
    void on_btnCircle_clicked();
    void on_btnTriangle_clicked();
    void on_btnRTriangle_clicked();

    void on_btnBorder_clicked();
    void on_btnFill_clicked();

private:
    EditViewer *__w;
    QColor prevColor=Qt::red;

    BaseButton *btnBorder;
    BaseButton *btnFill;
    BaseButton *btnRectangle;
    BaseButton *btnCircle;
    BaseButton *btnTriangle;
    BaseButton *btnRTriangle;
};

#endif

#ifndef PENTOOL_H
#define PENTOOL_H

#include "basetool.h"

class EditViewer;
class QPushButton;
class ColorPanel;
class BaseButton;

class PenTool : public BaseTool
{
    Q_OBJECT

public:
    explicit PenTool(QWidget *parent = nullptr);
    ~PenTool();
    void singlePressed();
    void initUI();
	QColor color();
	int penSize();

public slots:
    void on_btnPoint_clicked();
    void on_btnLine_clicked();
    void on_btnDashLine_clicked();
    void on_btnCurve_clicked();
    void on_combPenSize_currentTextChanged(const QString &);

private:
    EditViewer *__w;
    QPushButton *btnMainColor;
    ColorPanel *colorPanel;
    QColor m_color = Qt::red;
    int m_penSize = 2;
    BaseButton *btnPoint;
    BaseButton *btnLine;
    BaseButton *btnCurve;
    BaseButton *btnDashLine;
};

#endif

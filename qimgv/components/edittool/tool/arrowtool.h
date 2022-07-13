#ifndef ARROWTOOL_H
#define ARROWTOOL_H

#include "basetool.h"

class EditViewer;
class BaseButton;

class ColorPanel;
class ArrowTool : public BaseTool
{
    Q_OBJECT
public:
    ArrowTool(QWidget *parent=nullptr);
    void initUI();
    void singlePressed();
    QColor color();
    int penSize();
public slots:
    void on_triArrow1_clicked();
    void on_lineArrow2_clicked();
private:
    void on_combPenSize_currentTextChanged(const QString& text);
    EditViewer *__w;
    BaseButton * btnArrow1;
    BaseButton * btnArrow2;
    ColorPanel* colorPanel;
    QColor m_color = Qt::red;
    int m_penSize = 2;
};

class BrushTool : public BaseTool
{
	Q_OBJECT
public:
	BrushTool(QWidget* parent = nullptr);
	void initUI();
	QColor color();
private:
    EditViewer* __w;
    ColorPanel* colorPanel;
    QColor m_color = Qt::red;
};


#endif

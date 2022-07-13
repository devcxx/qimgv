#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>

class EditViewer;
class CustomTextEdit;

class TextEdit : public QTextEdit
{
	Q_OBJECT
public:
    TextEdit(QWidget* parent);
    void draw();
public slots:
	void textAreaChanged();
private:
    EditViewer *m_imageView;
};


class InputTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit InputTextEdit(QWidget *parent = nullptr);
    ~InputTextEdit();
    void paintEvent(QPaintEvent*);
    void moveEvent(QMoveEvent*);

    QPair<QStringList,QVector<QPoint>> getMultilineText(const QString &);
    CustomTextEdit *customTextEdit;
public slots:
    void textUpdated(const QString &);
private:
    EditViewer* w;

    QString textValue;
    QPoint prevPos;
};

#endif

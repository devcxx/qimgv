#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "basetool.h"

class EditViewer;
class BaseButton;
class BaseComboBox;

class QPushButton;

class TextTool : public BaseTool
{
    Q_OBJECT

public:
    explicit TextTool(QWidget *parent = nullptr);
    ~TextTool();
    void setButtonColor(QColor);
    void initUI();
    QFont currentFont();

public slots:
    void setCurrentColor();
    void setCurrentFont(const QString& fontFamily, int fontSize);
    void on_combFontFamily_currentTextChanged(const QString &);
    void on_combFontSize_currentTextChanged(const QString &);
    void on_btnTextBold_clicked();
    void on_btnTextUnderline_clicked();
    void on_btnTextItalic_clicked();
    void on_btnTextStrikeout_clicked();

private:
    EditViewer *__w;

    BaseButton *btnTextBold;
    BaseButton *btnTextItalic;
    BaseButton *btnTextUnderline;
    BaseButton *btnTextStrikeout;

    BaseComboBox *combFontFamily;
    BaseComboBox *combFontSize;
    QPushButton *btnColor;

    QString m_fontFamily;
    int m_fontSize = 10;
    QColor m_color = Qt::red;
    bool m_bBold = false;
    bool m_underline = false;
    bool m_itatlic = false;
    bool m_strikout = false;
};

#endif

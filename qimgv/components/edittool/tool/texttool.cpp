#include "../widget/customtextedit.h"
#include "../widget/basebutton.h"
#include "../widget/basecombobox.h"
#include "../widget/separator.h"
#include "../editeviewer.h"
#include "texttool.h"

#include <QHBoxLayout>
#include <QColorDialog>
#include <QPainter>

TextTool::TextTool(QWidget *parent) :
    BaseTool(parent),
    __w((EditViewer*)parent)
{
    setFixedSize(480,50);
    initUI();
}

void TextTool::initUI()
{
    QHBoxLayout *hboxL=new QHBoxLayout(this);
    hboxL->setMargin(0);
    hboxL->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hboxL->setSpacing(0);

    QHBoxLayout *hL_Options=new QHBoxLayout();
    hL_Options->setMargin(0);
    hL_Options->setContentsMargins(0,0,0,0);
    hL_Options->setSpacing(0);

    combFontFamily=new BaseComboBox(tr("Font families"),this);
    combFontSize=new BaseComboBox(tr("Font size"),this);

    combFontSize->setFixedWidth(50);

    combFontFamily->addItems(qCore->getFontFamilies());
    for(int i=10;i<20;i+=2) combFontSize->addItem(QString::number(i));
    for(int i=20;i<50;i+=4) combFontSize->addItem(QString::number(i));
    for(int i=50;i<72;i+=8) combFontSize->addItem(QString::number(i));

    QFont defaultFont=qCore->getAppFont();
    combFontSize->insertItem(0,QString::number(defaultFont.pointSize()));
    combFontSize->setCurrentText("10");
    combFontFamily->setCurrentText(defaultFont.family());

    btnTextBold=new BaseButton(qCore->pngImagePath()+QStringLiteral("bold@2x.png"),tr("Bold"),this);
    btnTextUnderline=new BaseButton(qCore->pngImagePath()+QStringLiteral("underline@2x.png"),tr("Underline"),this);
    btnTextItalic=new BaseButton(qCore->pngImagePath()+QStringLiteral("italic@2x.png"),tr("Italic"),this);
    btnTextStrikeout=new BaseButton(qCore->pngImagePath()+QStringLiteral("strikeout@2x.png"),tr("StrikeOut"),this);
    btnColor=new QPushButton(this);
    btnColor->setCursor(Qt::PointingHandCursor);

    hL_Options->addWidget(btnTextBold);
    hL_Options->addWidget(btnTextUnderline);
    hL_Options->addWidget(btnTextItalic);
    hL_Options->addWidget(btnTextStrikeout);

    hboxL->addLayout(hL_Options);
    hboxL->addWidget(new Separator(this));
    hboxL->addWidget(combFontFamily);
    hboxL->addWidget(combFontSize);
    hboxL->addWidget(new Separator(this));
    hboxL->addWidget(btnColor);

    setButtonColor(Qt::red);
    __w->inputTextEdit->customTextEdit->setTextColor(Qt::red);
    __w->m_edit->setTextColor(Qt::red);
    __w->m_edit->setFontPointSize(10);

    setLayout(hboxL);

    connect(combFontFamily,&BaseComboBox::currentTextChanged,this,&TextTool::on_combFontFamily_currentTextChanged);
    connect(combFontSize,&BaseComboBox::currentTextChanged,this,&TextTool::on_combFontSize_currentTextChanged);

    connect(btnTextBold,&BaseButton::clicked,this,&TextTool::on_btnTextBold_clicked);
    connect(btnTextUnderline,&BaseButton::clicked,this,&TextTool::on_btnTextUnderline_clicked);
    connect(btnTextItalic,&BaseButton::clicked,this,&TextTool::on_btnTextItalic_clicked);
    connect(btnTextStrikeout,&BaseButton::clicked,this,&TextTool::on_btnTextStrikeout_clicked);
    connect(btnColor,&QPushButton::clicked,this,&TextTool::setCurrentColor);
    
    combFontFamily->setCurrentText("Microsoft YaHei");
    m_fontFamily = "Microsoft YaHei";
    combFontFamily->setStyleSheet("BaseComboBox{ background-color: #383838; color: white; }");
    combFontSize->setStyleSheet("BaseComboBox{ background-color: #383838; color: white; }");
}

TextTool::~TextTool() {}

QFont TextTool::currentFont()
{
    QFont f;
    f.setFamily(m_fontFamily);
    f.setBold(m_bBold);
    f.setPointSize(m_fontSize);
    f.setStrikeOut(m_strikout);
    f.setItalic(m_itatlic);
    f.setUnderline(m_underline);
    return f;
}


void TextTool::setButtonColor(QColor color)
{
   QString styleSheetFmt = QString("QPushButton{border:2px solid black;background-color:%1}");
   btnColor->setStyleSheet(styleSheetFmt.arg(color.name()));
}

void TextTool::setCurrentColor()
{
    QColor color=QColorDialog::getColor(Qt::red,this,QString(),QColorDialog::ShowAlphaChannel);
    if(color.spec()!=QColor::Invalid) {
        setButtonColor(color);
        m_color = color;
		__w->m_edit->setTextColor(color);
        __w->m_edit->setText(__w->m_edit->toPlainText());
    }
}

void TextTool::setCurrentFont(const QString &fontFamily, int fontSize)
{
//     combFontFamily->setCurrentText(fontFamily);
//     combFontSize->setCurrentText(QString::number(fontSize));
}

void TextTool::on_combFontFamily_currentTextChanged(const QString &)
{
    qCore->setFont(QFont(combFontFamily->currentText(),combFontSize->currentText().toInt()));
    qCore->getFont().setBold(btnTextBold->isChecked());
    qCore->getFont().setUnderline(btnTextUnderline->isChecked());
    qCore->getFont().setItalic(btnTextItalic->isChecked());
    qCore->getFont().setStrikeOut(btnTextStrikeout->isChecked());

    m_fontFamily = combFontFamily->currentText();
    m_fontSize = combFontSize->currentText().toInt();
    m_bBold = btnTextBold->isChecked();
    m_underline = btnTextUnderline->isChecked();
    m_itatlic = btnTextItalic->isChecked();
    m_strikout = btnTextStrikeout->isChecked();

    __w->m_edit->setFontPointSize(combFontSize->currentText().toInt());
    __w->m_edit->setFont(qCore->getFont());
    __w->m_edit->setText(__w->m_edit->toPlainText());
}

void TextTool::on_combFontSize_currentTextChanged(const QString &)
{
    on_combFontFamily_currentTextChanged(combFontFamily->currentText());
}

void TextTool::on_btnTextBold_clicked()
{
    on_combFontSize_currentTextChanged(combFontSize->currentText());
}

void TextTool::on_btnTextUnderline_clicked()
{
    on_combFontSize_currentTextChanged(combFontSize->currentText());
}

void TextTool::on_btnTextItalic_clicked()
{
    on_combFontSize_currentTextChanged(combFontSize->currentText());
}

void TextTool::on_btnTextStrikeout_clicked()
{
    on_combFontSize_currentTextChanged(combFontSize->currentText());
}


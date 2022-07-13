#include "../editeviewer.h"
#include "customtextedit.h"
#include "inputtextedit.h"

#include <QPainter>


TextEdit::TextEdit(QWidget* parent) :
    QTextEdit(parent),
    m_imageView((EditViewer*)parent)
{
	setFixedSize(100, 50);
	connect(this, SIGNAL(textChanged()), this, SLOT(textAreaChanged()));
	setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setLineWrapMode(LineWrapMode::NoWrap);
	setStyleSheet("TextEdit{background-color: transparent ; border:2px solid gray;}");
	setFocus();
}

void TextEdit::textAreaChanged()
{
	QTextDocument* document = this->document();
	auto height = document->size().height() + 2;
	height = height > 50 ? height : 50;
	auto width = document->size().width();
	width = width > 100 ? width : 100;
    auto m_region = m_imageView->region;
	setFixedWidth(width);
	setFixedHeight(height);
	auto deltax = m_region.x() + m_region.width() - (x() + size().width());
	auto deltay = m_region.y() + m_region.height() - (y() + size().height());
	if (deltax < 0) {
		move(-deltax > x() - m_region.x() ? m_region.x() : x() + deltax, y());
	}
	if (deltay < 0) {
		move(x(), -deltay > y() - m_region.y() ? m_region.y() : y() + deltay);
	}
	auto cur = textCursor();
    if (width > m_region.width())
        setFixedWidth(m_region.width());
    if (height > m_region.height())
        setFixedHeight(m_region.height());
}

void TextEdit::draw()
{
	auto text = toPlainText();
	if (m_imageView->shapeType == ShapeType::Text && !text.trimmed().isEmpty()) {
		shape_object so;
		so.st = m_imageView->shapeType;
		so.color = textColor();
        so.font = m_imageView->textTool->currentFont();
		auto x = text.split("\n");
		so.ts = x;
		QVector<QPoint> points;
		points << pos();
		so.ps = points;
		m_imageView->sos.push_back(so);
		m_imageView->update();
	}
}

InputTextEdit::InputTextEdit(QWidget *parent) :
    QWidget(parent),
    w((EditViewer*)parent)
{

    setWindowFlag(Qt::FramelessWindowHint);
    customTextEdit=new CustomTextEdit(this);

    connect(customTextEdit,&CustomTextEdit::textUpdated,this,&InputTextEdit::textUpdated);
    connect(customTextEdit,&CustomTextEdit::TextEditSizeChanged,this,[&](const QRect&r){
        this->setFixedSize(r.width(),r.height());
    });
}


QPair<QStringList, QVector<QPoint>> InputTextEdit::getMultilineText(const QString &s)
{
    QStringList sl=s.split('\n');
    auto fm=customTextEdit->fontMetrics();
    int h=fm.height();

    QPoint np(prevPos);
    QVector<QPoint> lps;

    for(int i=0; i<sl.size(); i++){
        QPoint pos(np.x(),np.y()+h*(i+1));
        lps.push_back(pos);
    }
    return qMakePair(sl,lps);
}

void InputTextEdit::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event);

    if(w->shapeType==ShapeType::Text && !textValue.trimmed().isEmpty()){
        shape_object so;
        so.st=w->shapeType;
        so.color=customTextEdit->textColor();
        so.font=qCore->getFont();
        auto x=getMultilineText(textValue);
        so.ts=x.first;
        so.ps=x.second;
        w->sos.push_back(so);
        w->update();
    }
    QWidget::moveEvent(event);
}


void InputTextEdit::textUpdated(const QString& text)
{
    textValue=text;
    prevPos=mapToParent(customTextEdit->pos());
}

InputTextEdit::~InputTextEdit()
{
}

void InputTextEdit::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QPen(qCore->getBorderColor(),2));
    p.drawRect(rect());
    p.fillRect(rect(),QColor(0,0,0,100));
    QWidget::paintEvent(event);
}


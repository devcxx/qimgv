#include "arrowtool.h"
#include "../editeviewer.h"
#include "../widget/colorpanel.h"
#include "../widget/separator.h"
#include "../widget/basebutton.h"
#include "../widget/basecombobox.h"

ArrowTool::ArrowTool(QWidget *parent)
    : BaseTool(parent),
    __w((EditViewer*)parent)

{
    setFixedSize(290,50);
    initUI();
}
  
void ArrowTool::initUI()
{
//     QHBoxLayout *hBoxlayout=new QHBoxLayout(this);
//     hBoxlayout->setMargin(0);
//     hBoxlayout->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
//     hBoxlayout->setSpacing(0);
// 
//     btnArrow1=new BaseButton(qCore->pngImagePath()+"tri-arrow@2x.png",tr("Triangle Arrow"),this);
//     btnArrow2=new BaseButton(qCore->pngImagePath()+"arrow@2x.png",tr("Line Arrow"),this);
//     hBoxlayout->addWidget(btnArrow1);
//     hBoxlayout->addWidget(btnArrow2);
//     setLayout(hBoxlayout);
// 
//     connect(btnArrow1,&BaseButton::clicked,this,&ArrowTool::on_triArrow1_clicked);
//     connect(btnArrow2,&BaseButton::clicked,this,&ArrowTool::on_lineArrow2_clicked);

	QValidator* validator = new QRegExpValidator(QRegExp("[0-9]\\d{0,1}", Qt::CaseInsensitive), this);
	BaseComboBox* combPenSize = new BaseComboBox(tr("Pen size"), this);
	combPenSize->setValidator(validator);
	combPenSize->setFixedWidth(50);
	for (int i = 1; i < 10; i += 1) combPenSize->addItem(QString::number(i));
	for (int i = 10; i < 30; i += 4) combPenSize->addItem(QString::number(i));
	for (int i = 30; i < 50; i += 5) combPenSize->addItem(QString::number(i));
	for (int i = 50; i <= 72; i += 6) combPenSize->addItem(QString::number(i));
	combPenSize->setCurrentIndex(1);
	combPenSize->setEditable(true);
	combPenSize->setMaxVisibleItems(10);
	combPenSize->setStyleSheet("BaseComboBox{background-color: #383838; color: white;}");

    connect(combPenSize, &QComboBox::currentTextChanged, this, &ArrowTool::on_combPenSize_currentTextChanged);

	colorPanel = new ColorPanel(this);
	connect(colorPanel, &ColorPanel::getColor, this, [&](const QColor& color) {
            m_color = color;
		});

	QHBoxLayout* hboxL = new QHBoxLayout(this);
	hboxL->setMargin(0);
	/*hboxL->setContentsMargins(qCore->borderPadding() / 2, 0, qCore->borderPadding() / 2, 0);*/
	hboxL->setSpacing(4);

	QHBoxLayout* hL_Options = new QHBoxLayout();
	hboxL->addWidget(colorPanel);
	hboxL->addWidget(new Separator(this));
	hboxL->addWidget(combPenSize);
	hboxL->addWidget(new Separator(this));

	hL_Options->setMargin(0);
	hL_Options->setContentsMargins(0, 0, 0, 0);
	hL_Options->setSpacing(0);

	btnArrow1=new BaseButton(qCore->pngImagePath()+"tri-arrow@2x.png",tr("Triangle Arrow"),this);
	btnArrow2=new BaseButton(qCore->pngImagePath()+"arrow@2x.png",tr("Line Arrow"),this);

    connect(btnArrow1,&BaseButton::clicked,this,&ArrowTool::on_triArrow1_clicked);
    connect(btnArrow2,&BaseButton::clicked,this,&ArrowTool::on_lineArrow2_clicked);

	hL_Options->addWidget(btnArrow1);
	hL_Options->addWidget(btnArrow2);

	hboxL->addLayout(hL_Options);
	setLayout(hboxL);
}




void ArrowTool::on_combPenSize_currentTextChanged(const QString& text)
{
	if (text.isEmpty()) return;
	int penSize = text.toInt();
	if (penSize <= 0) m_penSize = 0;
	else if (penSize >= 30) m_penSize = 30;
	else m_penSize = penSize;
}


void ArrowTool::singlePressed()
{
    btnArrow1->setChecked(false);
    btnArrow2->setChecked(false);
    switch (qCore->getSingleShape()){
    case ShapeType::TriArrow: btnArrow1->setChecked(true);break;
    case ShapeType::LineArrow: btnArrow2->setChecked(true);break;
    default:break;
    }
}

void ArrowTool::on_triArrow1_clicked()
{
    __w->shapeType=ShapeType::TriArrow;
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
}

void ArrowTool::on_lineArrow2_clicked()
{
    __w->shapeType=ShapeType::LineArrow;
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
}

QColor ArrowTool::color()
{
	return m_color;
}

int ArrowTool::penSize()
{
	return m_penSize;
}


BrushTool::BrushTool(QWidget* parent)
	: BaseTool(parent),
    __w((EditViewer*)parent)

{
	setFixedSize(120, 50);
	initUI();
}

void BrushTool::initUI()
{
	colorPanel = new ColorPanel(this);
    connect(colorPanel, &ColorPanel::getColor, this, [this](const QColor &color){
		m_color = color;
		});

	QHBoxLayout* hboxL = new QHBoxLayout(this);
	hboxL->setMargin(0);
	hboxL->setContentsMargins(qCore->borderPadding() / 2, 0, qCore->borderPadding() / 2, 0);
	hboxL->setSpacing(4);
	hboxL->addWidget(colorPanel);
	setLayout(hboxL);
}

QColor BrushTool::color()
{
    return m_color;
}

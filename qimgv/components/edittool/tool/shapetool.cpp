#include "../widget/colorpanel.h"
#include "../widget/separator.h"
#include "../widget/basebutton.h"

#include "../editeviewer.h"
#include "shapetool.h"
#include <QHBoxLayout>
#include <QPainter>

ShapeTool::ShapeTool(QWidget *parent) :
    BaseTool(parent),
    __w((EditViewer*)parent)
{
    setFixedSize(395,50);

    initUI();
}

void ShapeTool::initUI()
{
    QHBoxLayout *hboxL=new QHBoxLayout(this);
    hboxL->setMargin(0);
    hboxL->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hboxL->setSpacing(4);

    QHBoxLayout *hL_Options=new QHBoxLayout();
    hL_Options->setMargin(0);
    hL_Options->setContentsMargins(0,0,0,0);
    hL_Options->setSpacing(4);

    Separator *separator=new Separator(this);
    ColorPanel *colorPanel=new ColorPanel(this);
    connect(colorPanel,&ColorPanel::getColor,this,[&](const QColor &color){
        prevColor=color;
        if(__w->fill)
            qCore->setBgColor(color);
        else
            qCore->setPenColor(color);
    });
    hboxL->insertWidget(0,colorPanel);
    hboxL->addWidget(new Separator(this));
    btnBorder=new BaseButton(qCore->pngImagePath()+QStringLiteral("border-rect@2x.png"),tr("Border"),this);
    btnFill=new BaseButton(qCore->pngImagePath()+QStringLiteral("filled-rect@2x.png"),tr("Fill"),this);
    btnRectangle=new BaseButton(qCore->pngImagePath()+QStringLiteral("rectangular@2x.png"),tr("Rectangle"),this);
    btnCircle=new BaseButton(qCore->pngImagePath()+QStringLiteral("circle@2x.png"),tr("Circle"),this);
    btnTriangle=new BaseButton(qCore->pngImagePath()+QStringLiteral("triangle@2x.png"),tr("Triangle"),this);
    btnRTriangle=new BaseButton(qCore->pngImagePath()+QStringLiteral("rtriangle@2x.png"),tr("Vertical triangle"),this);
    hL_Options->addWidget(btnBorder);
    hL_Options->addWidget(btnFill);
    hL_Options->addWidget(btnRectangle);
    hL_Options->addWidget(btnCircle);
    hL_Options->addWidget(btnTriangle);
    hL_Options->addWidget(btnRTriangle);
    hL_Options->insertWidget(2,separator);

    hboxL->addLayout(hL_Options);
    setLayout(hboxL);

    connect(btnBorder,SIGNAL(clicked(bool)),this,SLOT(on_btnBorder_clicked()));
    connect(btnFill,SIGNAL(clicked(bool)),this,SLOT(on_btnFill_clicked()));
    connect(btnRectangle,SIGNAL(clicked(bool)),this,SLOT(on_btnRectangle_clicked()));
    connect(btnCircle,SIGNAL(clicked(bool)),this,SLOT(on_btnCircle_clicked()));
    connect(btnTriangle,SIGNAL(clicked(bool)),this,SLOT(on_btnTriangle_clicked()));
    connect(btnRTriangle,SIGNAL(clicked(bool)),this,SLOT(on_btnRTriangle_clicked()));
    btnBorder->setChecked(true);
}

ShapeTool::~ShapeTool() {}

void ShapeTool::singlePressed()
{
    btnRectangle->setChecked(false);
    btnCircle->setChecked(false);
    btnTriangle->setChecked(false);
    btnRTriangle->setChecked(false);
    if(qCore->getSingleShape()==ShapeType::Rectangle)
        btnRectangle->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::Circle)
        btnCircle->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::Triangle)
        btnTriangle->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::RTriangle)
        btnRTriangle->setChecked(true);
}

void ShapeTool::on_btnRectangle_clicked()
{
    __w->shapeType=ShapeType::Rectangle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnCircle_clicked()
{
    __w->shapeType=ShapeType::Circle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnTriangle_clicked()
{
    __w->shapeType=ShapeType::Triangle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnRTriangle_clicked()
{
    __w->shapeType=ShapeType::RTriangle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnBorder_clicked()
{
    qCore->setPenColor(prevColor);
    __w->fill=false;
    btnBorder->setChecked(true);
    btnFill->setChecked(false);
}

void ShapeTool::on_btnFill_clicked()
{
    qCore->setBgColor(prevColor);
    __w->fill=true;
    btnBorder->setChecked(false);
    btnFill->setChecked(true);
}

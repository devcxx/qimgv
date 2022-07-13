
#include "erasetool.h"
#include "../editeviewer.h"
#include "../widget/separator.h"
#include "../widget/basebutton.h"
#include "../widget/basecombobox.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QButtonGroup>

EraseTool::EraseTool(QWidget *parent)
    : BaseTool(parent),
      __w((EditViewer*)parent),
      eraseType(EraseType::None)
{
    setFixedSize(155,50);

    initUI();
}

void EraseTool::initUI()
{
    QHBoxLayout* hBoxLayout=new QHBoxLayout(this);
    hBoxLayout->setMargin(0);
    hBoxLayout->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hBoxLayout->setSpacing(4);

    btnCircleErase=new BaseButton(qCore->pngImagePath()+"erase-rectangular@2x.png",tr("General erase"),this);
    btnRegionCircle=new BaseButton(qCore->pngImagePath()+"erase-rectangular0@2x.png",tr("Region erase"),this);
    auto group = new QButtonGroup(this);
    group->addButton(btnRegionCircle);
    group->addButton(btnCircleErase);
    btnCircleErase->setCheckable(true);

    combEraseSize=new BaseComboBox(tr("Erase size"),this);
    for(int i=10; i<=50; i+=10) combEraseSize->addItem(QString::number(i));
    combEraseSize->setStyleSheet("BaseComboBox{ background-color:#383838;color: white;}");

    hBoxLayout->addWidget(combEraseSize);
	hBoxLayout->addWidget(new Separator(this));
    hBoxLayout->addWidget(btnCircleErase);
    hBoxLayout->addWidget(btnRegionCircle);

    setLayout(hBoxLayout);

    connect(combEraseSize,&BaseComboBox::currentTextChanged,this,[&](){
        qCore->setEraseSize(combEraseSize->currentText().toInt());
        __w->eraseSize=qCore->getEraseSize();
    });
//    connect(btnCircleErase,&BaseButton::clicked,this,[&](){
//        eraseType=EraseType::General;
//        __w->egeneral=true;
//        __w->shapeType=ShapeType::Erase;
//        singlePressed();
//    });
//    connect(btnRegionCircle,&BaseButton::clicked,this,[&](){
//        eraseType=EraseType::Region;
//        __w->egeneral=false;
//        __w->shapeType=ShapeType::Erase;
//        singlePressed();
//    });
}

void EraseTool::on_btnRegionCircle_clicked()
{
    eraseType=EraseType::Region;
    __w->egeneral=false;
    __w->shapeType=ShapeType::Erase;
    singlePressed();
}

void EraseTool::on_btnCircleErase_clicked()
{
    eraseType=EraseType::General;
    __w->egeneral=true;
    __w->shapeType=ShapeType::Erase;
    singlePressed();
}

void EraseTool::singlePressed()
{
    btnCircleErase->setChecked(false);
    btnRegionCircle->setChecked(false);
    if(eraseType==EraseType::General)
        btnCircleErase->setChecked(true);
    else if(eraseType==EraseType::Region)
        btnRegionCircle->setChecked(true);
}

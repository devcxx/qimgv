#include "toolwidget.h"
//#include "helper/imageuploader.h"
#include "basebutton.h"
#include "../editeviewer.h"
#include "pinwidget.h"
#include <QHBoxLayout>
#include <QPainter>

ToolWidget::ToolWidget(QWidget* parent)
    : BaseTool(parent)
    , captureW((EditViewer*)parent)
{
//    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(550, 50);

    initUI();
}

void ToolWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.setBrush(QBrush(qCore->getWidgetBgColor()));
    p.setPen(Qt::transparent);

    p.drawRoundedRect(rect(), 10, 10);

    QWidget::paintEvent(event);
}

void ToolWidget::initUI()
{
    //    auto dsEffect = new QGraphicsDropShadowEffect(this);
    //    dsEffect->setBlurRadius(5);
    //    dsEffect->setOffset(0);
    //    dsEffect->setColor(qCore->getWidgetBgColor());
    //    setGraphicsEffect(dsEffect);

    QHBoxLayout *hboxL=new QHBoxLayout(this);
    hboxL->setMargin(0);
    hboxL->setContentsMargins(qCore->borderPadding()/2,qCore->borderPadding()/2,qCore->borderPadding()/2,qCore->borderPadding()/2);
    hboxL->setSpacing(0);

    btnShape=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_1graph@2x.png"),tr("shape tool"),this);
    btnPen=new BaseButton(qCore->pngImagePath()+QStringLiteral("pen@2x.png"),tr("pen tool"),this);
    btnText=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_2text@2x.png"),tr("text tool"),this);
    btnArrow=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_3arrow@2x.png"),tr("arrow tool"),this);
    btnBlur=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_4paint@2x.png"),tr("blur"),this);
    btnBrush=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_5brush@2x.png"),tr("Brush tool"),this);
    btnErase=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_6eraser@2x.png"),tr("erase tool"),this);
    btnUndo=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_7revocation@2x.png"),tr("undo"),this,false);
    btnRedo=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_8redo@2x.png"),tr("redo"),this,false);
    btnCancel=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_10close@2x.png"),tr("close"),this,false);
    btnPin=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_9pig@2x.png"),tr("pin tool"),this,false);
    btnSave=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_11save@2x.png"),tr("save to file"),this,false);
    btnCopy=new BaseButton(qCore->pngImagePath()+QStringLiteral("icon_12paste@2x.png"),tr("copy to clipboard"),this,false);

    hboxL->addWidget(btnShape);
    hboxL->addWidget(btnPen);
    hboxL->addWidget(btnText);
    hboxL->addWidget(btnArrow);
    hboxL->addWidget(btnBlur);
    hboxL->addWidget(btnBrush);
    hboxL->addWidget(btnErase);
    hboxL->addWidget(btnUndo);
    hboxL->addWidget(btnRedo);
    hboxL->addWidget(btnPin);
    hboxL->addWidget(btnCancel);
    hboxL->addWidget(btnSave);
    hboxL->addWidget(btnCopy);
    setLayout(hboxL);

    connect(btnShape,SIGNAL(clicked(bool)),this,SLOT(on_btnShape_clicked()));
    connect(btnPen,SIGNAL(clicked(bool)),this,SLOT(on_btnPen_clicked()));
    connect(btnText,SIGNAL(clicked(bool)),this,SLOT(on_btnText_clicked()));
    connect(btnArrow,SIGNAL(clicked(bool)),this,SLOT(on_btnArrow_clicked()));
    connect(btnErase,SIGNAL(clicked(bool)),this,SLOT(on_btnErase_clicked()));
    connect(btnUndo,SIGNAL(clicked(bool)),this,SLOT(on_btnUndo_clicked()));
    connect(btnRedo,SIGNAL(clicked(bool)),this,SLOT(on_btnRedo_clicked()));
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(on_btnCancel_clicked()));
    connect(btnSave,SIGNAL(clicked(bool)),this,SLOT(on_btnSave2File_clicked()));
    connect(btnCopy,SIGNAL(clicked(bool)),this,SLOT(on_btnCopyClipboard_clicked()));
    connect(btnBlur,SIGNAL(clicked(bool)),this,SLOT(on_btnBlur_clicked()));
    connect(btnBrush,SIGNAL(clicked(bool)),this,SLOT(on_btnBrush_clicked()));
    connect(btnPin,SIGNAL(clicked(bool)),this,SLOT(on_btnPin_clicked()));

    btnErase->setVisible(false);
}

ToolWidget::~ToolWidget() {}

void ToolWidget::singlePressed()
{
    btnShape->setChecked(false);
    btnPen->setChecked(false);
    btnErase->setChecked(false);
    btnText->setChecked(false);
    btnArrow->setChecked(false);
    btnBlur->setChecked(false);
    btnBrush->setChecked(false);
    switch (captureW->paintEventNotify) {
    case PaintEventNotify::Shape: {
        btnShape->setChecked(true);
        captureW->shapeTool->on_btnBorder_clicked();
        captureW->shapeTool->on_btnCircle_clicked();
        captureW->m_edit->hide();
        captureW->m_edit->clear();
        break;
    }
    case PaintEventNotify::Pen: {
        btnPen->setChecked(true);
        captureW->penTool->on_btnCurve_clicked();
        captureW->m_edit->hide();
        captureW->m_edit->clear();
        break;
    }
    case PaintEventNotify::Erase: {
        btnErase->setChecked(true);
        captureW->eraseTool->on_btnRegionCircle_clicked();
        captureW->m_edit->hide();
        captureW->m_edit->clear();
        break;
    }
    case PaintEventNotify::Text: {
        btnText->setChecked(true);
        captureW->textTool->setCurrentFont("Microsoft YaHei", 10);
        break;
    }
    case PaintEventNotify::Arrow: {
        btnArrow->setChecked(true);
        captureW->arrowTool->on_lineArrow2_clicked();
        captureW->m_edit->hide();
        captureW->m_edit->clear();
        break;
    }
    case PaintEventNotify::Blur:
        btnBlur->setChecked(true);
        captureW->m_edit->hide();
        captureW->m_edit->clear();
        break;
    case PaintEventNotify::Brush:
        btnBrush->setChecked(true);
        captureW->m_edit->hide();
        captureW->m_edit->clear();
        break;
    default:
        break;
    }
}

void ToolWidget::triggerInputTextEdit()
{
    captureW->inputTextEdit->move(0, 0);
    captureW->inputTextEdit->hide();
    captureW->update();
}

void ToolWidget::on_btnBrush_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Brush;
    captureW->painting = true;
    captureW->resizing = false;

    triggerInputTextEdit();
    captureW->shapeType = ShapeType::Brush;
    qCore->setSingleShape(captureW->shapeType);

    captureW->eraseTool->hide();
    captureW->penTool->hide();
    captureW->textTool->hide();
    captureW->arrowTool->hide();
    captureW->shapeTool->hide();
    captureW->brushTool->show();
    captureW->moveWidgetTool(captureW->brushTool);

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnSave2File_clicked()
{
    captureW->setCursor(Qt::ArrowCursor);

    if (captureW->paintEventNotify == PaintEventNotify::Text) {
        //captureW->inputTextEdit->hide();
        captureW->m_edit->hide();
    }
    captureW->grabSubRegion();
    QString filename = qCore->PixMap2ImageFile(qCore->getPixMap());
    if (!filename.isEmpty() && qCore->imageReadonly()) {
        qCore->showInFolder(filename);
        on_btnCancel_clicked();
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(filename);
    }

    if (!qCore->imageReadonly()) {
        emit captureW->editDone();
    }

    //    qCore->getSysTray()->showMessage(tr("QtScreenShot notification"),tr("The image has been saved to a file"));
    // copy filepath to clipboard

    //    if(qCore->getEnableUpload()){
    //        UploaderThread *th=new UploaderThread(filename);
    //        th->start();
    //        th->wait(10);
    //    }
}

void ToolWidget::on_btnCopyClipboard_clicked()
{
    captureW->setCursor(Qt::ArrowCursor);

    if (captureW->paintEventNotify == PaintEventNotify::Text) {
        //captureW->inputTextEdit->hide();
        captureW->m_edit->hide();
    }
    captureW->grabSubRegion();
    qCore->PixMap2ClipBoard(qCore->getPixMap());
    //    qCore->getSysTray()->showMessage(tr("QtScreenShot notification"),tr("The image has been saved to the clipboard"));
    on_btnCancel_clicked();
}

void ToolWidget::on_btnPin_clicked()
{
    auto r = captureW->grabSubRegion();
    pinWidget = new PinWidget(qCore->getPixMap());
    connect(pinWidget, &PinWidget::closed, this, [this] { pinWidgetVisiable = false; });
    pinWidget->setGeometry(r);
    pinWidget->setAttribute(Qt::WA_DeleteOnClose);
    pinWidget->show();
    pinWidgetVisiable = true;
    on_btnCancel_clicked();
}

void ToolWidget::on_btnCancel_clicked()
{
    //    kCapture_window_load=false;
    captureW->setCursor(Qt::ArrowCursor);
    //    captureW->close();
    if (qCore->imageReadonly()) {
        emit captureW->editCancel();
        close();
    } else {
        emit captureW->editDone();
    }
}

void ToolWidget::on_btnBlur_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Blur;
    captureW->painting = true;
    captureW->resizing = false;

    triggerInputTextEdit();
    captureW->shapeType = ShapeType::Blur;
    qCore->setSingleShape(captureW->shapeType);

    captureW->eraseTool->hide();
    captureW->penTool->hide();
    captureW->textTool->hide();
    captureW->arrowTool->hide();
    captureW->shapeTool->hide();
    captureW->brushTool->hide();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnShape_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Shape;
    captureW->painting = true;
    captureW->resizing = false;

    captureW->eraseTool->hide();
    captureW->penTool->hide();
    captureW->textTool->hide();
    captureW->arrowTool->hide();
    captureW->brushTool->hide();
    captureW->moveWidgetTool(captureW->shapeTool);
    captureW->shapeTool->show();
    triggerInputTextEdit();
    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnPen_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Pen;
    captureW->painting = true;
    captureW->resizing = false;

    captureW->eraseTool->hide();
    captureW->shapeTool->hide();
    captureW->textTool->hide();
    captureW->brushTool->hide();
    captureW->arrowTool->hide();
    captureW->moveWidgetTool(captureW->penTool);
    captureW->penTool->show();
    triggerInputTextEdit();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnText_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Text;
    captureW->shapeType = ShapeType::Text;
    qCore->setSingleShape(captureW->shapeType);
    captureW->painting = true;
    captureW->resizing = false;

    captureW->eraseTool->hide();
    captureW->shapeTool->hide();
    captureW->penTool->hide();
    captureW->arrowTool->hide();
    captureW->brushTool->hide();
    captureW->moveWidgetTool(captureW->textTool);
    captureW->textTool->show();
    triggerInputTextEdit();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnArrow_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Arrow;
    captureW->painting = true;
    captureW->resizing = false;

    captureW->eraseTool->hide();
    captureW->shapeTool->hide();
    captureW->textTool->hide();
    captureW->penTool->hide();
    captureW->brushTool->hide();
    captureW->arrowTool->show();
    triggerInputTextEdit();

    captureW->moveWidgetTool(captureW->arrowTool);
    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnErase_clicked()
{
    captureW->paintEventNotify = PaintEventNotify::Erase;

    triggerInputTextEdit();
    captureW->shapeType = ShapeType::Unknown;
    captureW->painting = true;
    captureW->resizing = false;

    captureW->eraseTool->show();
    captureW->textTool->hide();
    captureW->shapeTool->hide();
    captureW->penTool->hide();
    captureW->brushTool->hide();
    captureW->arrowTool->hide();

    captureW->moveWidgetTool(captureW->eraseTool);
    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnUndo_clicked()
{
    triggerInputTextEdit();

    captureW->textTool->hide();
    captureW->resetPaintInfo();

    if (!captureW->sos.empty()) {
        restore.push(captureW->sos.back());
        captureW->sos.pop_back();
        captureW->update();
    }
}

void ToolWidget::on_btnRedo_clicked()
{
    triggerInputTextEdit();
    captureW->textTool->hide();

    if (!restore.empty()) {
        captureW->sos.push_back(restore.top());
        restore.pop();
        captureW->update();
    }
}

void ToolWidget::setViewMode(bool bIsWindowView)
{
    if (bIsWindowView) {
        btnErase->hide();
    }
}

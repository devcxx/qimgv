#ifndef ERASETOOL_H
#define ERASETOOL_H

#include "../core/core_system.h"
#include "basetool.h"

class EditViewer;
class BaseButton;
class BaseComboBox;
class EraseTool : public BaseTool
{
    Q_OBJECT
public:

    enum class EraseType{
        None,
        General,
        Region
    };

    explicit EraseTool(QWidget *parent = nullptr);

    void singlePressed();
    void initUI();

public slots:
    void on_btnRegionCircle_clicked();
    void on_btnCircleErase_clicked();

private:
    EditViewer *__w;
    BaseButton * btnCircleErase;
    BaseButton * btnRegionCircle;
    BaseComboBox * combEraseSize;

    EraseType eraseType;
};

#endif

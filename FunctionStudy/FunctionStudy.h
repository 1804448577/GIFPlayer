#pragma once

#include <QtWidgets/QWidget>
#include "ui_FunctionStudy.h"

class GIFTEST : public QWidget
{
    Q_OBJECT

public:
    GIFTEST(QWidget *parent = nullptr);
    ~GIFTEST();

private:
    Ui::GIFTESTClass ui;
};

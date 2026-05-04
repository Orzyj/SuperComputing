#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SuperComputing.h"

class SuperComputing : public QMainWindow
{
    Q_OBJECT

public:
    SuperComputing(QWidget *parent = nullptr);
    ~SuperComputing();

private:
    Ui::SuperComputingClass ui;
};


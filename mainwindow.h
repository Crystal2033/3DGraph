#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QLayout>
#include "My3DWidget.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:
    //METHODS
    void setBackColor(const QColor& color);


    //VARIABLES
    Ui::mainwindowClass ui;
    QVBoxLayout* mainLayout = nullptr;
    QPalette* palette = nullptr;
    My3DWidget* engineWidget = nullptr;
};
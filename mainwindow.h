#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QLayout>
#include "My3DWidget.h"
#include <qlabel.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include "AxisSlider.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:
    void setBackColor(const QColor& color);
    void setAxisSliders();
    AxisSlider* xAxisSlider = nullptr;
    AxisSlider* yAxisSlider = nullptr;
    AxisSlider* zAxisSlider = nullptr;
    void setXAxisSlider();
    void setYAxisSlider();
    void setZAxisSlider();

    Ui::mainwindowClass ui;
    QVBoxLayout* mainLayout = nullptr;

    QHBoxLayout* menuHorLayout = nullptr;

    QPalette* palette = nullptr;
    My3DWidget* engineWidget = nullptr;
};

#include "mainwindow.h"


mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setBackColor(Qt::lightGray);

    mainLayout = new QVBoxLayout();
    ui.setupUi(this);
    engineWidget = new My3DWidget(this);
    mainLayout->addWidget(engineWidget);
    this->centralWidget()->setLayout(mainLayout);
    menuHorLayout = new QHBoxLayout();
 
    setAxisSliders();
    mainLayout->addLayout(menuHorLayout);
}

mainwindow::~mainwindow()
{
    if (palette != nullptr) {
        delete palette;
    }
}

void mainwindow::setBackColor(const QColor& color)
{
    palette = new QPalette;
    palette->setColor(QPalette::Window, color);

    this->setAutoFillBackground(true);
    this->setPalette(*palette);
}


void mainwindow::setAxisSliders() {
    setXAxisSlider();
    setYAxisSlider();
    setZAxisSlider();
}

void mainwindow::setXAxisSlider() {
    xAxisSlider = new AxisSlider();
    xAxisSlider->createAxisSlider(*menuHorLayout, 'X');
    xAxisSlider->addObserver(engineWidget);
}

void mainwindow::setYAxisSlider() {
    yAxisSlider = new AxisSlider();
    yAxisSlider->createAxisSlider(*menuHorLayout, 'Y');
    yAxisSlider->addObserver(engineWidget);
}

void mainwindow::setZAxisSlider() {
    zAxisSlider = new AxisSlider();
    zAxisSlider->createAxisSlider(*menuHorLayout, 'Z');
    zAxisSlider->addObserver(engineWidget);
}

void mainwindow::keyPressEvent(QKeyEvent* pe) {
    engineWidget->keyPressEvent(pe);
}
#include "mainwindow.h"


mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    setBackColor(Qt::darkGray);

    mainLayout = new QVBoxLayout();
    ui.setupUi(this);
    engineWidget = new My3DWidget(this);
    mainLayout->addWidget(engineWidget);
    this->centralWidget()->setLayout(mainLayout);
}

mainwindow::~mainwindow()
{
    delete palette;
}

void mainwindow::setBackColor(const QColor& color)
{
    palette = new QPalette;
    palette->setColor(QPalette::Window, color);

    this->setAutoFillBackground(true);
    this->setPalette(*palette);
}

#include "AxisSlider.h"

void AxisSlider::createAxisSlider(QBoxLayout& layout, const char axisName) {

    this->axisName = axisName;
    axisHorLay = new QHBoxLayout();

    axisLabel = new QLabel(QString(this->axisName));
    axisSlider = new QSlider(Qt::Horizontal);
    axisSlider->setRange(-180, 180);
    axisSlider->setValue(0);
    axisSlider->setTickPosition(QSlider::TicksBelow);
    
    axisNumGadget = new QLCDNumber();
    axisNumGadget->display(value);
    axisNumGadget->setFixedSize(70, 70);
    
    axisHorLay->addWidget(axisLabel, 1);
    axisHorLay->addWidget(axisSlider, 6);
    axisHorLay->addWidget(axisNumGadget, 1);

    layout.addLayout(axisHorLay);
    connect(axisSlider, SIGNAL(valueChanged(int)), SLOT(onValueChanged(int)));
}

void AxisSlider::onValueChanged(int val) {
    value = val;
    axisNumGadget->display(value);
    notifyObservers();
}

void AxisSlider::notifyObservers() {
    for (auto obs : observers) {
        obs->updateObserver(float(value), axisName);
    }
}

void AxisSlider::addObserver(InterfaceObserver* observer) {
    this->observers.append(observer);
}

void AxisSlider::removeObserver(InterfaceObserver* observer) {
    QList<InterfaceObserver*>::ConstIterator it = observers.constBegin();
    for (; it != observers.constEnd(); ++it) {
        if (*it == observer) {
            observers.erase(it);
            return;
        }
    }
}


int AxisSlider::getValue() const {
    return value;
}
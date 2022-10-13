#pragma once
#include <QLayout>
#include <qlabel.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include "subject.h"

class AxisSlider : public QWidget, public InterfaceSubject
{
    Q_OBJECT
public:
    AxisSlider() = default;
    void createAxisSlider(QBoxLayout& layout, const char axisName);
    int getValue() const;
    ~AxisSlider() = default;
    void addObserver(InterfaceObserver* observer) override;
    void removeObserver(InterfaceObserver* observer) override;

private slots:
    void onValueChanged(int val);
    
    void notifyObservers();


private:
    int value = 0;
    char axisName;
    QLabel* axisLabel = nullptr;
    QSlider* axisSlider = nullptr;
    QLCDNumber* axisNumGadget = nullptr;
    QHBoxLayout* axisHorLay = nullptr;
    QList<InterfaceObserver*> observers;
};
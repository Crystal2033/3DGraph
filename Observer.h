#ifndef OBSERVER_H
#define OBSERVER_H
#include <QImage>

class InterfaceObserver
{
public:
    virtual ~InterfaceObserver() = default;
    virtual void updateObserver(const float value, const char axisName) = 0;
};

//class HistogramObserver : public InterfaceObserver
//{
//public:
//    HistogramObserver();
//    ~HistogramObserver() = default;
//    void update() override;
//};

#endif // OBSERVER_H

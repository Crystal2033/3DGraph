#ifndef SUBJECT_H
#define SUBJECT_H
#pragma once
#include "observer.h"

class InterfaceSubject
{
public:
    virtual ~InterfaceSubject() = default;
    virtual void addObserver(InterfaceObserver* observer) = 0;
    virtual void removeObserver(InterfaceObserver* observer) = 0;
    virtual void notifyObservers() = 0;
};

//class ImageSubject : public InterfaceSubject
//{
//public:
//    ImageSubject();
//    void addObserver(InterfaceObserver* observer) override;
//    void removeObserver(InterfaceObserver* observer) override;
//    void notify() override;
//    ~ImageSubject() = default;

//};

#endif // SUBJECT_H

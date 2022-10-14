#pragma once
#include <QWidget>
#include <iostream>
#include <QPalette.h>
#include <qpainter.h>
#include "graphicPrimitives.h"
#include "TransformMatrix.h"
#include <algorithm>
#include "Observer.h"
#include <QKeyEvent>

#define THETA_ADDITIONAL 0.005f

class My3DWidget  : public QWidget, public InterfaceObserver
{
	Q_OBJECT

public:
	My3DWidget(QWidget *parent);
	void keyPressEvent(QKeyEvent* pe) override;
private:
	void updateObserver(const float value, const char axisName);

	void setBackColor(const QColor& color);
	void paintEvent(QPaintEvent* event) override;

	void makeProjTransform();

	float getVisibleNumber(const GraphicPrimitives::Triangle& triangle);
	void drawTriangle(const GraphicPrimitives::Triangle& trianle, const Qt::PenStyle style);

	void drawTriangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2);
	void initFigure();

	GraphicObject myFigure;
	float fTheta = 0.0;
	float xFTheta = 0.0;
	float yFTheta = 0.0;
	float zFTheta = 0.0;

	float fYaw = 0.0;

	glm::vec4 camera = {0, 0, 0 ,0};
	glm::vec4 lookDirection = { 0, 0, 0 ,0 };
	glm::vec4 vecForward = {0,0,0,0};
	

	~My3DWidget();

	QPalette* palette = nullptr;
	QPainter painter;
	QPen pen;


};

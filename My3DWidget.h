#pragma once
#include <QWidget>
#include <iostream>
#include <QPalette.h>
#include <qpainter.h>
#include "graphicPrimitives.h"
#include "TransformMatrix.h"
#include <algorithm>
#define THETA_ADDITIONAL 0.005

class My3DWidget  : public QWidget
{
	Q_OBJECT

public:
	My3DWidget(QWidget *parent);

private:
	void setBackColor(const QColor& color);
	void paintEvent(QPaintEvent* event) override;

	void makeProjTransform();

	float getVisibleNumber(const GraphicPrimitives::Triangle& triangle);
	void drawTriangle(const GraphicPrimitives::Triangle& trianle, const Qt::PenStyle style);

	void drawTriangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2);
	glm::vec4 makeMultiplication(const glm::vec4& point, const glm::mat4x4& matrix);
	void initFigure();

	GraphicObject myFigure;
	float fTheta = 0.0;

	glm::vec4 camera = {0, 0, 0 , 0};

	~My3DWidget();

	QPalette* palette = nullptr;
	QPainter painter;
	QPen pen;


};

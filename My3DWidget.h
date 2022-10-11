#pragma once
#include <QWidget>
#include <iostream>
#include <QPalette.h>
#include <qpainter.h>
#include "graphicPrimitives.h"
#include <chrono>
#include "TransformMatrix.h"
#define THETA_ADDITIONAL 0.002

class My3DWidget  : public QWidget
{
	Q_OBJECT

public:
	My3DWidget(QWidget *parent);

private:
	//METHODS
	std::chrono::system_clock clock;
	void setBackColor(const QColor& color);
	void paintEvent(QPaintEvent* event) override;
	void createLine();
	void makeProjTransform();
	void updateRotateMatrixes();
	void drawTriangle(const GraphicPrimitives::Triangle& trianle, const Qt::PenStyle style);

	void drawTriangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2);
	glm::vec4 makeMultiplication(const glm::vec4& point, const glm::mat4x4& matrix);
	void initFigure();

	GraphicObject myFigure;
	float fTheta = 0.0;
	glm::mat4x4 matrixProj = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	glm::vec3 camera = { 0,0,0 };

	RotateMatrix* xRotateMatrix = nullptr;
	RotateMatrix* yRotateMatrix = nullptr;
	RotateMatrix* zRotateMatrix = nullptr;

	TransformMatrix* projectionMatrix = nullptr;
	//// Rotation X
	//rotateMatrixX[0][0] = 1;
	//rotateMatrixX[1][1] = cosf(fTheta);
	//rotateMatrixX[1][2] = sinf(fTheta);
	//rotateMatrixX[2][1] = -sinf(fTheta);
	//rotateMatrixX[2][2] = cosf(fTheta);
	//rotateMatrixX[3][3] = 1;

	//// Rotation Y
	//rotateMatrixY[0][0] = cosf(fTheta);
	//rotateMatrixY[0][2] = sinf(fTheta);
	//rotateMatrixY[1][1] = 1;
	//rotateMatrixY[2][0] = -sinf(fTheta);
	//rotateMatrixY[2][2] = cosf(fTheta);
	//rotateMatrixX[3][3] = 1;

	//// Rotation Z
	//rotateMatrixZ[0][0] = cosf(fTheta);
	//rotateMatrixZ[0][1] = sinf(fTheta);
	//rotateMatrixZ[1][0] = -sinf(fTheta);
	//rotateMatrixZ[1][1] = cosf(fTheta);
	//rotateMatrixZ[2][2] = 1;
	//rotateMatrixZ[3][3] = 1;

	glm::mat4x4 rotateMatrixX = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	glm::mat4x4 rotateMatrixY = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	glm::mat4x4 rotateMatrixZ = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	~My3DWidget();

	//VARIABLES
	QPalette* palette = nullptr;
	QPainter painter;
	QPen pen;


};

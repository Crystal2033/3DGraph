#include "My3DWidget.h"
#include<QDebug>


My3DWidget::My3DWidget(QWidget *parent)
	: QWidget(parent)
{
	xRotateMatrix = new XRotateMatrix();
	yRotateMatrix = new YRotateMatrix();
	zRotateMatrix = new ZRotateMatrix();
	projectionMatrix = new ProjectionMatrix(0.1f, 1000.0f, 90.0f, (float)height() / (float)width());

	initFigure();
	setBackColor(Qt::black);
	pen.setColor(Qt::white);
}

void My3DWidget::setBackColor(const QColor& color)
{
	palette = new QPalette;
	palette->setColor(QPalette::Window, color);

	this->setAutoFillBackground(true);
	this->setPalette(*palette);
}

void My3DWidget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(pen);
	//painter.translate(width() / 2, height() / 2);
	makeProjTransform();
	update();
	painter.end();
}

void My3DWidget::makeProjTransform() {
	GraphicPrimitives::Triangle projectedTrianle;


	fTheta += 1.0f * THETA_ADDITIONAL;


	xRotateMatrix->updateMatrix(fTheta);
	yRotateMatrix->updateMatrix(fTheta);
	zRotateMatrix->updateMatrix(fTheta);
	

	for (auto& triangle : myFigure.mesh.triangles) {
		GraphicPrimitives::Triangle projectedTrianle, translatedTrianle, triangleRotatedZ, triangleRotatedXZ;

		for (int i = 0; i < 3; i++) {
			triangleRotatedZ.points[i] = zRotateMatrix->makeTransform(triangle.points[i]);
		}
		/*triangleRotatedZ.points[0] = zRotateMatrix->makeTransform(triangle.points[0]);
		triangleRotatedZ.points[1] = zRotateMatrix->makeTransform(triangle.points[1]);
		triangleRotatedZ.points[2] = zRotateMatrix->makeTransform(triangle.points[2]);*/

		for (int i = 0; i < 3; i++) {
			triangleRotatedXZ.points[i] = xRotateMatrix->makeTransform(triangleRotatedZ.points[i]);
		}
		/*triangleRotatedXZ.points[0] = xRotateMatrix->makeTransform(triangleRotatedZ.points[0]);
		triangleRotatedXZ.points[1] = xRotateMatrix->makeTransform(triangleRotatedZ.points[1]);
		triangleRotatedXZ.points[2] = xRotateMatrix->makeTransform(triangleRotatedZ.points[2]);*/

		translatedTrianle = triangleRotatedXZ;
		for (int i = 0; i < 3; i++) {
			translatedTrianle.points[i].z = triangleRotatedXZ.points[i].z + 5.0f;
		}
		glm::vec3 normal, line1, line2;
		line1.x = translatedTrianle.points[1].x - translatedTrianle.points[0].x;
		line1.y = translatedTrianle.points[1].y - translatedTrianle.points[0].y;
		line1.z = translatedTrianle.points[1].z - translatedTrianle.points[0].z;

		line2.x = translatedTrianle.points[2].x - translatedTrianle.points[0].x;
		line2.y = translatedTrianle.points[2].y - translatedTrianle.points[0].y;
		line2.z = translatedTrianle.points[2].z - translatedTrianle.points[0].z;

		normal = glm::cross(line1, line2);
		float normalLen = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x /= normalLen; normal.y /= normalLen; normal.z /= normalLen;
		/*translatedTrianle.points[0].z = triangleRotatedXZ.points[0].z + 3.0f;
		translatedTrianle.points[1].z = triangleRotatedXZ.points[1].z + 3.0f;
		translatedTrianle.points[2].z = triangleRotatedXZ.points[2].z + 3.0f;*/
		float visitedFactorNumber =
			normal.x * (translatedTrianle.points[0].x - camera.x) +
			normal.y * (translatedTrianle.points[0].y - camera.y) +
			normal.z * (translatedTrianle.points[0].z - camera.z);
									

		if (visitedFactorNumber < 0) {
			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i] = projectionMatrix->makeTransform(translatedTrianle.points[i]);
			}
			/*projectedTrianle.points[0] = projectionMatrix->makeTransform(translatedTrianle.points[0]);
			projectedTrianle.points[1] = projectionMatrix->makeTransform(translatedTrianle.points[1]);
			projectedTrianle.points[2] = projectionMatrix->makeTransform(translatedTrianle.points[2]);*/

			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i].x += 1.0f;
				projectedTrianle.points[i].y += 1.0f;
			}
			/*projectedTrianle.points[0].x += 1.0f; projectedTrianle.points[0].y += 1.0f;
			projectedTrianle.points[1].x += 1.0f; projectedTrianle.points[1].y += 1.0f;
			projectedTrianle.points[2].x += 1.0f; projectedTrianle.points[2].y += 1.0f;*/

			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i].x *= 0.5f * (float)width();
				projectedTrianle.points[i].y *= 0.5f * (float)height();
			}
			/*projectedTrianle.points[0].x *= 0.5f * (float)width();
			projectedTrianle.points[0].y *= 0.5f * (float)height();
			projectedTrianle.points[1].x *= 0.5f * (float)width();
			projectedTrianle.points[1].y *= 0.5f * (float)height();
			projectedTrianle.points[2].x *= 0.5f * (float)width();
			projectedTrianle.points[2].y *= 0.5f * (float)height();*/

			drawTriangle(projectedTrianle, Qt::SolidLine);
		}
	}
}

void My3DWidget::drawTriangle(const GraphicPrimitives::Triangle& trianle, const Qt::PenStyle style) {
	Q_UNUSED(style);
	painter.drawLine(trianle.points[0].x, trianle.points[0].y, trianle.points[1].x, trianle.points[1].y);
	painter.drawLine(trianle.points[1].x, trianle.points[1].y, trianle.points[2].x, trianle.points[2].y);
	painter.drawLine(trianle.points[2].x, trianle.points[2].y, trianle.points[0].x, trianle.points[0].y);
}

void My3DWidget::drawTriangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2) {
	painter.drawLine(x0, y0, x1, y1);
	painter.drawLine(x1, y1, x2, y2);
	painter.drawLine(x2, y2, x0, y0);
}

glm::vec4 My3DWidget::makeMultiplication(const glm::vec4& point, const glm::mat4x4& matrix) {
	glm::vec4 result = point * glm::transpose(matrix);

	if (result.w != 0.0f) {
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}

void My3DWidget::createLine() {
	//painter.drawLine(0, 0, 100, 100);
}

void My3DWidget::initFigure() {
	glm::vec4 south1[] = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) , glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)};
	glm::vec4 south2[] = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) , glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)};
			
	glm::vec4 east1[] = {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) , glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };
	glm::vec4 east2[] = {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) , glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) };
			
	glm::vec4 north1[] = { glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) , glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) };
	glm::vec4 north2[] = { glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) , glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) };
			
	glm::vec4 west1[] = { glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) , glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) };
	glm::vec4 west2[] = { glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) , glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
			
	glm::vec4 top1[] = { glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) , glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };
	glm::vec4 top2[] = { glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) , glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) };
			
	glm::vec4 bottom1[] = { glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) , glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
	glm::vec4 bottom2[] = { glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) , glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };

	/*this->myFigure.addNewTriangle(south1);
	this->myFigure.addNewTriangle(south2);

	this->myFigure.addNewTriangle(east1);
	this->myFigure.addNewTriangle(east2);

	this->myFigure.addNewTriangle(north1);
	this->myFigure.addNewTriangle(north2);

	this->myFigure.addNewTriangle(west1);
	this->myFigure.addNewTriangle(west2);

	this->myFigure.addNewTriangle(top1);
	this->myFigure.addNewTriangle(top2);

	this->myFigure.addNewTriangle(bottom1);
	this->myFigure.addNewTriangle(bottom2);*/
	this->myFigure.LoadFromObjectFile("PsevdoPyramid.obj");


	/*float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRation = (float)height() / (float)width();
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	matrixProj[0][0] = fAspectRation * fFovRad;
	matrixProj[1][1] = fFovRad;
	matrixProj[2][2] = fFar / (fFar - fNear);
	matrixProj[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrixProj[2][3] = 1.0f;
	matrixProj[3][3] = 0.0f;*/

	//matrixProj = glm::transpose(matrixProj);

}

My3DWidget::~My3DWidget()
{
	delete palette;
	delete xRotateMatrix;
	delete yRotateMatrix;
	delete zRotateMatrix;
	delete projectionMatrix;
}

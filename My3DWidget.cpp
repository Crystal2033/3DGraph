#include "My3DWidget.h"
#include<QDebug>


My3DWidget::My3DWidget(QWidget *parent)
	: QWidget(parent)
{
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

	glm::mat4x4 xRot = XRotateMatrix::getRotateMatrix(xFTheta * (M_PI / 180) );
	glm::mat4x4 yRot = YRotateMatrix::getRotateMatrix(yFTheta * (M_PI / 180));
	glm::mat4x4 zRot = ZRotateMatrix::getRotateMatrix(zFTheta * (M_PI / 180));
	
	//std::vector<GraphicPrimitives::Triangle> trianglesToRaster;
	glm::mat4x4 projMatrix = ProjectionMatrix::getProjMatrix(0.1f, 1000.0f, 90.0f, (float)height() / (float)width());
	/*glm::mat4x4 isoRotX = XRotateMatrix::getRotateMatrix(35.264 * (M_PI / 180));
	glm::mat4x4 isoRotY = YRotateMatrix::getRotateMatrix(45 * (M_PI / 180));
	glm::mat4x4 isoProjMatrix = isoRotX*isoRotY;*/
	
	glm::vec4 vecUp {0, 1, 0, 0};
	glm::vec4 vecTarget {0, 0, 1, 0};
	
	glm::mat4x4 matCameraRotate = YRotateMatrix::getRotateMatrix(fYaw * (M_PI / 180));
	lookDirection = glm::transpose(matCameraRotate) * vecTarget;
	vecTarget = camera + lookDirection;
	glm::mat4x4 cameraMatrix = matrixPointAt(camera, vecTarget, vecUp);

	//glm::mat4x4 matView = matrixQuickInverse(cameraMatrix);
	glm::mat4x4 matView = glm::inverse(cameraMatrix);

	vecForward = lookDirection * (8.0f * THETA_ADDITIONAL);
	

	for (auto& triangle : myFigure.mesh.triangles) {
		GraphicPrimitives::Triangle projectedTrianle, translatedTrianle, triangleRotatedXYZ, trianleViewed;

		glm::mat4x4 XYZRotation = xRot *yRot *zRot;
		
		for (int i = 0; i < 3; i++) {
			triangleRotatedXYZ.points[i] = triangle.points[i] * glm::transpose(XYZRotation);
		}
		
		glm::mat4x4 matTranslate = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.f, 0.f, 3.f));
		for (int i = 0; i < 3; i++) {
			translatedTrianle.points[i] = matTranslate * triangleRotatedXYZ.points[i];
		}


		if (getVisibleNumber(translatedTrianle) < 0) {

			for (int i = 0; i < 3; i++) {
				trianleViewed.points[i] = matView * translatedTrianle.points[i];
			}

			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i] = trianleViewed.points[i] * glm::transpose(projMatrix);

				if (projectedTrianle.points[i].w != 0.0f) {
					projectedTrianle.points[i] /= projectedTrianle.points[i].w;
				}
			}
			//for (int i = 0; i < 3; i++) {
			//	projectedTrianle.points[i] = glm::transpose(isoProjMatrix) * trianleViewed.points[i];
			//	/*if (projectedTrianle.points[i].w != 0.0f) {
			//		projectedTrianle.points[i] /= projectedTrianle.points[i].w;
			//	}*/
			//}

			glm::mat4x4 matTranslateWindow = glm::translate(glm::mat4x4(1.0f), glm::vec3(1.f, 1.f, 0.f));
			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i] = matTranslateWindow * projectedTrianle.points[i];
			}

			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i] = projectedTrianle.points[i] * glm::vec4(0.5f * (float)width(), 0.5f * (float)height(), 0.0f, 0.0f);
			}
			//trianglesToRaster.push_back(projectedTrianle);
			
			drawTriangle(projectedTrianle, Qt::SolidLine);
		}
		
	}
	
	/*std::sort(trianglesToRaster.begin(), trianglesToRaster.end(), [](GraphicPrimitives::Triangle& t1, GraphicPrimitives::Triangle& t2) {
		float middleZFirst = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
		float middleZSecond = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
		return middleZFirst > middleZSecond;
		});

	for (auto& triangle : trianglesToRaster) {
		drawTriangle(triangle, Qt::SolidLine);
	}*/
}

float My3DWidget::getVisibleNumber(const GraphicPrimitives::Triangle& triangle){
	glm::vec3 normal(1.0f), line1, line2;
	line1 = triangle.points[1] - triangle.points[0];

	line2 = triangle.points[2] - triangle.points[0];

	normal = glm::cross(line1,line2);
	normal = glm::normalize(normal);

	float visitedFactorNumber = glm::dot(normal, glm::vec3(triangle.points[0]) - glm::vec3(camera));

	return visitedFactorNumber;
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




void My3DWidget::initFigure() {
	this->myFigure.LoadFromObjectFile("SlidedPyramid4.obj");
}

void My3DWidget::updateObserver(const float value, const char axisName) {
	switch (axisName)
	{
	case 'X': {
		xFTheta = value;
		break;
	}
	case 'Y': {
		yFTheta = value;
		break;
	}
	case 'Z': {
		zFTheta = value;
		break;
	}
	default:
		exit(0);
	}
}

void My3DWidget::keyPressEvent(QKeyEvent* pe) {
	switch (pe->key())
	{
	case Qt::Key::Key_S: {
		camera.y += 8.0f * THETA_ADDITIONAL;
		break;
	}
	case Qt::Key::Key_W: {
		camera.y -= 8.0f * THETA_ADDITIONAL;
		break;
	}
	case Qt::Key::Key_D: {
		camera.x += 8.0f * THETA_ADDITIONAL;
		break;
	}
	case Qt::Key::Key_A: {
		camera.x -= 8.0f * THETA_ADDITIONAL;
		break;
	}
	case Qt::Key::Key_Q: {
		fYaw -= 100.0f * THETA_ADDITIONAL;
		break;
	}
	case Qt::Key::Key_E: {
		fYaw += 100.0f * THETA_ADDITIONAL;
		break;
	}
	case Qt::Key::Key_F: {
		camera = camera + vecForward;
		break;
	}
	case Qt::Key::Key_R: {
		camera = camera - vecForward;
		break;
	}
	default:
		break;
	}
}

My3DWidget::~My3DWidget(){
	delete palette;
}




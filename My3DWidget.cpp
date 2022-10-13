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

	fTheta += 1.0f * THETA_ADDITIONAL;

	glm::mat4x4 xRot = XRotateMatrix::getRotateMatrix(fTheta*0.5f);
	glm::mat4x4 yRot = YRotateMatrix::getRotateMatrix(fTheta);
	glm::mat4x4 zRot = ZRotateMatrix::getRotateMatrix(fTheta);
	
	//std::vector<GraphicPrimitives::Triangle> trianglesToRaster;
	glm::mat4x4 projMatrix = ProjectionMatrix::getProjMatrix(0.1f, 1000.0f, 90.0f, (float)height() / (float)width());

	for (auto& triangle : myFigure.mesh.triangles) {
		GraphicPrimitives::Triangle projectedTrianle, translatedTrianle, triangleRotatedXZ;

		glm::mat4x4 XZRotation = xRot *zRot  ;
		
		for (int i = 0; i < 3; i++) {
			triangleRotatedXZ.points[i] = triangle.points[i] * glm::transpose(XZRotation);
		}

		translatedTrianle = triangleRotatedXZ;
		
		glm::mat4x4 matTranslate = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.f, 0.f, 8.f));
		for (int i = 0; i < 3; i++) {
			translatedTrianle.points[i] = matTranslate * translatedTrianle.points[i];
		}

		if (getVisibleNumber(translatedTrianle) < 0) {
			for (int i = 0; i < 3; i++) {
				projectedTrianle.points[i] = translatedTrianle.points[i] * glm::transpose(projMatrix);

				if (projectedTrianle.points[i].w != 0.0f) {
					projectedTrianle.points[i] /= projectedTrianle.points[i].w;
				}
			}
			
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

glm::vec4 My3DWidget::makeMultiplication(const glm::vec4& point, const glm::mat4x4& matrix) {
	glm::vec4 result = point * glm::transpose(matrix);

	if (result.w != 0.0f) {
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}


void My3DWidget::initFigure() {
	this->myFigure.LoadFromObjectFile("Ship.txt");
}

My3DWidget::~My3DWidget(){
	delete palette;
}

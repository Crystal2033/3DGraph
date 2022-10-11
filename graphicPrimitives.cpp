#include "graphicPrimitives.h"

void GraphicObject::addNewTriangle(glm::vec4 points[3]) {
	mesh.triangles.push_back(GraphicPrimitives::Triangle(points));
}
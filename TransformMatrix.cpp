#include "TransformMatrix.h"
void XRotateMatrix::updateMatrix(float fTheta) {
	matrix = {
			1,0,0,0,
			0,cosf(fTheta),sinf(fTheta),0,
			0,-sinf(fTheta),cosf(fTheta),0,
			0,0,0,1
	};
}

void YRotateMatrix::updateMatrix(float fTheta) {
	matrix = {
			cosf(fTheta),0, -sinf(fTheta),0,
			0,1,0,0,
			sinf(fTheta),0, cosf(fTheta),0,
			0,0,0,1
	};
}


void ZRotateMatrix::updateMatrix(float fTheta) {
	matrix = {
			cosf(fTheta), sinf(fTheta),0,0,
			-sinf(fTheta), cosf(fTheta),0,0,
			0,0,1,0,
			0,0,0,1
	};
}

ProjectionMatrix::ProjectionMatrix(const float zNear, const float zFar, const float fovAngle, const float aRatio) {
	float fFovRad = 1.0f / tanf(fovAngle * 0.5f / 180.0f * 3.14159f);

	matrix[0][0] = aRatio * fFovRad;
	matrix[1][1] = fFovRad;
	matrix[2][2] = zFar / (zFar - zNear);
	matrix[3][2] = (-zFar * zNear) / (zFar - zNear);
	matrix[2][3] = 1.0f;
	matrix[3][3] = 0.0f;
}

glm::vec4 ProjectionMatrix::makeTransform(const glm::vec4& points) const {
	glm::vec4 result = points * glm::transpose(matrix);

	if (result.w != 0.0f) {
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}


glm::vec4 RotateMatrix::makeTransform(const glm::vec4& points) const {
	return glm::vec4(points * glm::transpose(matrix));
}
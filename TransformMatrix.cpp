#include "TransformMatrix.h"
#include <math.h>
#define M_PI 3.14159265358979323846
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
			cosf(fTheta),0, sinf(fTheta),0,
			0,1,0,0,
			-sinf(fTheta),0, cosf(fTheta),0,
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

glm::mat4x4 XRotateMatrix::getRotateMatrix(float fTheta) {
	return glm::mat4x4(
			1,0,0,0,
			0,cosf(fTheta),sinf(fTheta),0,
			0,-sinf(fTheta),cosf(fTheta),0,
			0,0,0,1
	);
}

glm::mat4x4 YRotateMatrix::getRotateMatrix(float fTheta) {
	return glm::mat4x4(
		cosf(fTheta), 0, sinf(fTheta), 0,
		0, 1, 0, 0,
		-sinf(fTheta), 0, cosf(fTheta), 0,
		0, 0, 0, 1
	);
}


glm::mat4x4 ZRotateMatrix::getRotateMatrix(float fTheta) {
	return glm::mat4x4(
		cosf(fTheta), sinf(fTheta), 0, 0,
		-sinf(fTheta), cosf(fTheta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

ProjectionMatrix::ProjectionMatrix(const float zNear, const float zFar, const float fovAngle, const float aRatio) {
	float fFovRad = 1.0f / tanf(fovAngle * 0.5f / 180.0f * M_PI);

	matrix[0][0] = aRatio * fFovRad;
	matrix[1][1] = fFovRad;
	matrix[2][2] = zFar / (zFar - zNear);
	matrix[3][2] = (-zFar * zNear) / (zFar - zNear);
	matrix[2][3] = 1.0f;
	matrix[3][3] = 0.0f;
}

glm::mat4x4 ProjectionMatrix::getProjMatrix(const float zNear, const float zFar, const float fovAngle, const float aRatio) {
	float fFovRad = 1.0f / tanf(fovAngle * 0.5f / 180.0f * M_PI);
	glm::mat4x4 projMatrix(0.0f);

	projMatrix[0][0] = aRatio * fFovRad;
	projMatrix[1][1] = fFovRad;
	projMatrix[2][2] = zFar / (zFar - zNear);
	projMatrix[3][2] = (-zFar * zNear) / (zFar - zNear);
	projMatrix[2][3] = 1.0f;
	projMatrix[3][3] = 0.0f;
	return projMatrix;
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


glm::mat4x4 matrixPointAt(glm::vec4 pos, glm::vec4 target, glm::vec4 up) {
	glm::vec4 newForward = target - pos;
	newForward = glm::normalize(newForward);

	glm::vec4 a = newForward * glm::dot(up, newForward);
	glm::vec4 newUp = up - a;
	newUp = glm::normalize(newUp);

	glm::vec3 newRight = glm::cross(glm::vec3(newUp), glm::vec3(newForward));


	glm::mat4x4 matrix;
	matrix[0][0] = newRight.x;	matrix[0][1] = newRight.y;	matrix[0][2] = newRight.z;	matrix[0][3] = 0.0f;
	matrix[1][0] = newUp.x;		matrix[1][1] = newUp.y;		matrix[1][2] = newUp.z;		matrix[1][3] = 0.0f;
	matrix[2][0] = newForward.x;	matrix[2][1] = newForward.y;	matrix[2][2] = newForward.z;	matrix[2][3] = 0.0f;
	matrix[3][0] = pos.x;			matrix[3][1] = pos.y;			matrix[3][2] = pos.z;			matrix[3][3] = 1.0f;
	return matrix;

}

glm::mat4x4 matrixQuickInverse(glm::mat4x4& m) // Only for Rotation/Translation Matrices
{
	glm::mat4x4 matrix;
	matrix[0][0] = m[0][0]; matrix[0][1] = m[1][0]; matrix[0][2] = m[2][0]; matrix[0][3] = 0.0f;
	matrix[1][0] = m[0][1]; matrix[1][1] = m[1][1]; matrix[1][2] = m[2][1]; matrix[1][3] = 0.0f;
	matrix[2][0] = m[0][2]; matrix[2][1] = m[1][2]; matrix[2][2] = m[2][2]; matrix[2][3] = 0.0f;
	matrix[3][0] = -(m[3][0] * matrix[0][0] + m[3][1] * matrix[1][0] + m[3][2] * matrix[2][0]);
	matrix[3][1] = -(m[3][0] * matrix[0][1] + m[3][1] * matrix[1][1] + m[3][2] * matrix[2][1]);
	matrix[3][2] = -(m[3][0] * matrix[0][2] + m[3][1] * matrix[1][2] + m[3][2] * matrix[2][2]);
	matrix[3][3] = 1.0f;
	return matrix;
}
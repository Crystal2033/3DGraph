#pragma once
#include <glm.hpp>
class TransformMatrix {
public:
	TransformMatrix() = default;
	virtual glm::vec4 makeTransform(const glm::vec4& points) const = 0;
	virtual ~TransformMatrix() = default;
protected:
	glm::mat4x4 matrix = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

};

//float fNear = 0.1f;
//float fFar = 1000.0f;
//float fFov = 90.0f;
//float fAspectRation = (float)height() / (float)width();
//float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
//
//matrixProj[0][0] = fAspectRation * fFovRad;
//matrixProj[1][1] = fFovRad;
//matrixProj[2][2] = fFar / (fFar - fNear);
//matrixProj[3][2] = (-fFar * fNear) / (fFar - fNear);
//matrixProj[2][3] = 1.0f;
//matrixProj[3][3] = 0.0f;

class ProjectionMatrix : public TransformMatrix {
public:
	ProjectionMatrix(const float zNear, const float zFar, const float fovAngle, const float aRatio);
	glm::vec4 makeTransform(const glm::vec4& points) const override;
	~ProjectionMatrix() = default;
};

class RotateMatrix : public TransformMatrix {
public:
	RotateMatrix() = default;
	glm::mat4x4 getMatrix() {
		return matrix;
	}
	virtual void updateMatrix(const float fTheta) = 0;
	glm::vec4 makeTransform(const glm::vec4& points) const override;
	virtual ~RotateMatrix() = default;

};

class XRotateMatrix : public RotateMatrix {
public:
	XRotateMatrix() = default;
	void updateMatrix(const float fTheta) override;
	~XRotateMatrix() = default;
};

class YRotateMatrix : public RotateMatrix {
public:
	YRotateMatrix() = default;
	void updateMatrix(const float fTheta) override;
	~YRotateMatrix() = default;
};

class ZRotateMatrix : public RotateMatrix {
public:
	ZRotateMatrix() = default;
	void updateMatrix(const float fTheta) override;
	~ZRotateMatrix() = default;
};
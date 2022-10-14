#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

class ProjectionMatrix : public TransformMatrix {
public:
	ProjectionMatrix(const float zNear, const float zFar, const float fovAngle, const float aRatio);
	static glm::mat4x4 getProjMatrix(const float zNear, const float zFar, const float fovAngle, const float aRatio);
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
	static glm::mat4x4 getRotateMatrix(const float fTheta);
	~XRotateMatrix() = default;
};

class YRotateMatrix : public RotateMatrix {
public:
	YRotateMatrix() = default;
	void updateMatrix(const float fTheta) override;
	static glm::mat4x4 getRotateMatrix(const float fTheta);
	~YRotateMatrix() = default;
};

class ZRotateMatrix : public RotateMatrix {
public:
	ZRotateMatrix() = default;
	void updateMatrix(const float fTheta) override;
	static glm::mat4x4 getRotateMatrix(const float fTheta);
	~ZRotateMatrix() = default;
};

glm::mat4x4 matrixPointAt(glm::vec4 pos, glm::vec4 target, glm::vec4 up);
glm::mat4x4 matrixQuickInverse(glm::mat4x4& m);
#include <glm.hpp>
#include <qvector.h>

class GraphicPrimitives {
public:
	class Triangle {
		
		public:
			glm::vec4 points[3];

			Triangle() = default;

			Triangle(const glm::vec4 points[3]) {
				this->points[0] = points[0];
				this->points[1] = points[1];
				this->points[2] = points[2];
			}

			~Triangle() = default;
	};

	class Mesh {
	public:
		QVector<Triangle> triangles;
	};
};

class GraphicObject {
public:
	GraphicObject() = default;
	~GraphicObject() = default;
	void addNewTriangle(glm::vec4 points[3]);

	GraphicPrimitives::Mesh mesh;
};
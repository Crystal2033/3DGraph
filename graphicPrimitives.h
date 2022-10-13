#include <glm.hpp>
#include <qvector.h>
#include <fstream>
#include <strstream>
#include <string>

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

	bool LoadFromObjectFile(const std::string sFilename)
	{
		std::ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		QVector<glm::vec4> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				glm::vec4 v;
				s >> junk >> v.x >> v.y >> v.z;
				v.w = 1;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				glm::vec4 vec[] = { glm::vec4(verts[f[0] - 1]), glm::vec4(verts[f[1] - 1]), glm::vec4(verts[f[2] - 1]) };

				mesh.triangles.push_back(GraphicPrimitives::Triangle(vec));
			}
		}

		return true;
	}
};
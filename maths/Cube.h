#pragma once
#include <GL/gl3w.h>
#include <vector>
#include <glm\glm.hpp>

class Cube
{

	GLuint vao; //vao
	GLuint positions; //vbo positions
	GLuint indices; //vbo indices

public:
	~Cube();

	std::vector<int> originalfaces =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3
	};

	std::vector<int> faces =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3
	};

	std::vector<float> originalVerts =
	{
		// front
		-2.0, -2.0,  2.0,
		2.0, -2.0,  2.0,
		2.0,  2.0,  2.0,
		-2.0,  2.0,  2.0,

		// back
		-2.0, -2.0, -2.0,
		2.0, -2.0, -2.0,
		2.0,  2.0, -2.0,
		-2.0,  2.0, -2.0
	};

	/*std::vector<float> verts =
	{
		// front
		-2.0, -2.0,  2.0,
		-1.0, 0.0, 0.0, 

		2.0, -2.0,  2.0,
		0.0, -1.0, 0.0,

		2.0,  2.0,  2.0,
		0.0, 0.0, 1.0,

		-2.0,  2.0,  2.0,
		0.0, 1.0, 0.0,

		// back
		-2.0, -2.0, -2.0,
		-1.0, 0.0, 0.0,

		2.0, -2.0, -2.0,
		1.0, 0.0, 0.0,

		2.0,  2.0, -2.0,
		1.0, 0.0, 0.0,

		-2.0,  2.0, -2.0,
		0.0, 0.0, -1.0
	};*/

	std::vector<float> verts =
	{
		// front
		-2.0, -2.0,  2.0,
		2.0, -2.0,  2.0,
		2.0,  2.0,  2.0,
		-2.0,  2.0,  2.0,
		// back
		-2.0, -2.0, -2.0,
		2.0, -2.0, -2.0,
		2.0,  2.0, -2.0,
		-2.0,  2.0, -2.0,
	};

	std::vector<float> previousVerts;
	std::vector<int> previousFaces;

	void restorePrevious();
	void savePrevious();
	void draw(bool wireframe);
	void reset();
	void initialize(int uPosition, int uNormal);
	
	glm::vec3 getNormalFromId(int id, std::vector<glm::vec4> faceNormals);
	std::vector<float> computeNormals();

};

// ? extern const GLuint PositionSlot;
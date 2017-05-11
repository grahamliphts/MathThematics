#pragma once
#include <GL/gl3w.h>
#include <vector>

class Cube
{
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

	std::vector<float> verts = 
	{
		// front
		-1.0, -1.0,  1.0, 
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0 
	};

	GLuint vao; //vao
	GLuint positions; //vbo positions
	GLuint indices; //vbo indices

public:
	~Cube();

	void draw() const;
	void initialize(int uPosition);
};

// ? extern const GLuint PositionSlot;
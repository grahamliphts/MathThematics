#include "Cube.h"

Cube::~Cube()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positions);
	glDeleteBuffers(1, &indices);
}

void Cube::draw() 
{
	glBindVertexArray(vao);
	//glDrawArrays(GL_POINTS, 0, faces.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void Cube::reset() 
{
	verts = originalVerts;
	faces = originalfaces;
}

void Cube::savePrevious()
{
	previousVerts = verts;
	previousFaces = faces;
}

void Cube::restorePrevious()
{
	verts = previousVerts;
	faces = previousFaces;
}

void Cube::initialize(int uPosition)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *  verts.size(), &verts[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(uPosition);
	glVertexAttribPointer(uPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(), &faces[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

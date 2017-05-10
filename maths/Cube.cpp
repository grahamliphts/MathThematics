#include "Cube.h"


Cube::~Cube()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positions);
	glDeleteBuffers(1, &indices);
}

void Cube::draw() const
{
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void Cube::initialize()
{
	IndexCount = sizeof(Faces) / sizeof(Faces[0]);

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &positions);
	glGenBuffers(1, &indices);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Faces), Faces, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(PositionSlot);
	//glVertexAttribPointer(PositionSlot, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

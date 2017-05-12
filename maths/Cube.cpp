#include "Cube.h"
#include "Subdivision.h"
#include <iostream>

Cube::~Cube()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positions);
	glDeleteBuffers(1, &indices);
}

void Cube::draw(bool wireframe) 
{
	glBindVertexArray(vao);
	if(wireframe)
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

void Cube::initialize(int uPosition, int uNormal)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::vector<float> newVerts = computeNormals();

	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *  newVerts.size(), &newVerts[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(uPosition);
	glVertexAttribPointer(uPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) 0);

	glEnableVertexAttribArray(uNormal);
	glVertexAttribPointer(uNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));


	glGenBuffers(1, &indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(), &faces[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


glm::vec3 Cube::getNormalFromId(int id, std::vector<glm::vec4> faceNormals)
{
	for (int i = 0; i < faceNormals.size(); i++)
	{
		if (faceNormals[i].w == id)
			return faceNormals[i];
	}
}

std::vector<float> Cube::computeNormals()
{
	std::vector<glm::vec4> facesNormals;

	std::vector<float> newVerts;

	for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
	{
		std::vector<int> idVertex;

		std::vector<glm::vec3> vertexFace = GetVertexInFaces(verts, faces, iFace / 3);

		glm::vec3 edge1 = vertexFace[2] - vertexFace[1];
		glm::vec3 edge2 = vertexFace[0] - vertexFace[1];

		std::cout << glm::cross(edge1, edge2).x << " " << glm::cross(edge1, edge2).y << " " << glm::cross(edge1, edge2).z << std::endl;
		facesNormals.push_back(glm::vec4(glm::normalize(glm::cross(edge1, edge2)), iFace));
	}
	for (int iVert = 0; iVert < verts.size() - 2; iVert += 3)
	{

		glm::vec3 currentVertex = glm::vec3(verts[iVert], verts[iVert + 1], verts[iVert + 2]);
		std::cout << currentVertex[0] << " " << currentVertex[1] << " " << currentVertex[2] << std::endl;

		glm::vec3 currentNormal = glm::vec3(0, 0, 0);
		std::vector<int> idFacesVertex = GetFacesFromVertex(faces, faces[iVert/3], -1);

		for (int i = 0; i < idFacesVertex.size(); i++)
		{
			glm::vec3 normal = getNormalFromId(idFacesVertex[i], facesNormals);
			currentNormal += glm::normalize(normal);
		}
		currentNormal /= idFacesVertex.size();

		newVerts.push_back(currentVertex[0]);
		newVerts.push_back(currentVertex[1]);
		newVerts.push_back(currentVertex[2]);
		newVerts.push_back(currentNormal[0]);
		newVerts.push_back(currentNormal[1]);
		newVerts.push_back(currentNormal[2]);
	}

	return newVerts;
}


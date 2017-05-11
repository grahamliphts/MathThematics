#include "Subdivision.h"

void SubdivisionKobbelt(std::vector<float> &vertex, std::vector<int> &faces, int iteration)
{
	for (int iter = 0; iter < iteration; iter++)
	{
		std::vector<float> newVertex;
		std::vector<int> newFaces;

		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
			std::vector<glm::vec3> vertexFace = getVertexInFaces(vertex, faces, iFace);
			glm::vec3 vertexCenter = (vertexFace[0] + vertexFace[1] + vertexFace[2]) / 3.0f;

			std::vector<int> adjFaces = getAdjacentFaces(faces, iFace);
			std::vector<glm::vec3> adjCenter;
			for (int i = 0; i < adjFaces.size(); i++)
			{
				std::vector<glm::vec3> vertexAdjFace = getVertexInFaces(vertex, faces, adjFaces[i]);
				glm::vec3 vertexCenterAdj = (vertexAdjFace[0] + vertexAdjFace[1] + vertexAdjFace[2]) / 3.0f;

				adjCenter.push_back(vertexCenterAdj);
			}
		}
	}
}

std::vector<glm::vec3> getVertexInFaces(std::vector<float> vertex, std::vector<int> faces, int idFace)
{
	std::vector<glm::vec3> vertexFace;

	vertexFace.push_back(glm::vec3(vertex[faces[idFace    ]], vertex[faces[idFace    ] + 1], vertex[faces[idFace    ] + 2]));
	vertexFace.push_back(glm::vec3(vertex[faces[idFace + 1]], vertex[faces[idFace + 1] + 1], vertex[faces[idFace + 1] + 2]));
	vertexFace.push_back(glm::vec3(vertex[faces[idFace + 2]], vertex[faces[idFace + 2] + 1], vertex[faces[idFace + 2] + 2]));

	return vertexFace;
}

std::vector<int> getAdjacentFaces(std::vector<int> faces, int idFace)
{
	std::vector<int> adjFaces;

	for (int i = 0; i < faces.size() - 2; i += 3)
	{
		if (i == idFace)
			continue;

		int commonVertex = 0;

		if (faces[idFace] == faces[i] || faces[idFace] == faces[i + 1] || faces[idFace] == faces[i + 2])
			commonVertex++;
		if (faces[idFace + 1] == faces[i] || faces[idFace + 1] == faces[i + 1] || faces[idFace + 1] == faces[i + 2])
			commonVertex++;
		if (faces[idFace + 2] == faces[i] || faces[idFace + 2] == faces[i + 1] || faces[idFace + 2] == faces[i + 2])
			commonVertex++;

		if (commonVertex == 2)
			adjFaces.push_back(i);
	}

	return adjFaces;
}
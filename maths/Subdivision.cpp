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

			int nbPoint = 6;
			glm::vec3 newVertexCenter = (1 - Perturbation(nbPoint)) * vertexCenter;

			glm::vec3 somme(0, 0, 0);
			for (int i = 0; i < vertexFace.size(); i++)
				somme += vertexFace[i];
			for (int i = 0; i < adjCenter.size(); i++)
				somme += adjCenter[i];

			newVertexCenter = newVertexCenter + (Perturbation(nbPoint) / nbPoint) * somme;
		}
	}
}

void SubdivisionLoop(std::vector<float> &vertex, std::vector<int> &faces, int iteration)
{
	/*for (int iter = 0; iter < iteration; iter++)
	{
		std::vector<float> newVertex;
		std::vector<int> newFaces;


		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
		}

		for (int iVertex = 0; iVertex < vertex.size() - 2; iVertex +=3)
		{
			glm::vec3 currentVertex = glm::vec3(vertex[iVertex], vertex[iVertex +1], vertex[iVertex +2]);
			getAdjacentVertex(currentVertex);


			glm::vec3 somme(0, 0, 0);
			for (int iAdjVertex = 0; iAdjVertex < adjVertex.size(); iAdjVertex++)
				somme += adjVertex[iAdjVertex];
			glm::vec3 newVertex = (1 - Alpha(adjVertex.size() * currentVertex)) + Alpha(adjVertex.size()) * somme;
		}
	}*/
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

float Perturbation(int n)
{
	return (1 / 9.0f) * (4 - 2 * cos(2 * M_PI / n));
}

float Alpha(int n)
{
	float alpha;
	if (n == 3)
		alpha = 3 / 16;
	else if (n > 3)
		alpha = 1 / n * (5 / 8 - pow((3 / 8 + 1 / 4 * cos(2 * M_PI / n)), 2));
}
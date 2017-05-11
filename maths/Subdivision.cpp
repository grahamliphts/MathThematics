#include "Subdivision.h"

void SubdivisionKobbelt(std::vector<float> &vertex, std::vector<int> &faces, int iteration)
{
	for (int iter = 0; iter < iteration; iter++)
	{
		std::vector<float> newVertexCenter;
		std::vector<float> newVertex = GetPertubedVertexKobbelt(vertex, faces);
		std::vector<int> newFaces;

		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
			std::vector<glm::vec3> vertexFace = GetVertexInFaces(vertex, faces, iFace);
			glm::vec3 vertexCenter = (vertexFace[0] + vertexFace[1] + vertexFace[2]) / 3.0f;

			std::vector<int> adjFaces = GetAdjacentFaces(faces, iFace);
			std::vector<glm::vec3> adjCenter;
			for (int i = 0; i < adjFaces.size(); i++)
			{
				std::vector<glm::vec3> vertexAdjFace = GetVertexInFaces(vertex, faces, adjFaces[i]);
				glm::vec3 vertexCenterAdj = (vertexAdjFace[0] + vertexAdjFace[1] + vertexAdjFace[2]) / 3.0f;

				adjCenter.push_back(vertexCenterAdj);
			}


			/*
			int nbPoint = 6;
			glm::vec3 newVertexCenter = (1 - Perturbation(nbPoint)) * vertexCenter;

			glm::vec3 somme(0, 0, 0);
			for (int i = 0; i < vertexFace.size(); i++)
				somme += vertexFace[i];
			for (int i = 0; i < adjCenter.size(); i++)
				somme += adjCenter[i];

			newVertexCenter = newVertexCenter + (Perturbation(nbPoint) / nbPoint) * somme;*/
		}
	}
}

void SubdivisionLoop(std::vector<float> &vertex, std::vector<int> &faces, int iteration)
{
	for (int iter = 0; iter < iteration; iter++)
	{
		std::vector<float> newVertex;
		std::vector<int> newFaces;

		/*for (int iVertex = 0; iVertex < vertex.size() - 2; iVertex ++)
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
		}*/
	}
}

std::vector<int> GetIdVertexInFaces(std::vector<int> faces, int idFace)
{
	std::vector<int> vertexId;

	vertexId.push_back(faces[idFace]);
	vertexId.push_back(faces[idFace + 1]);
	vertexId.push_back(faces[idFace + 2]);

	return vertexId;
}

std::vector<glm::vec3> GetVertexInFaces(std::vector<float> vertex, std::vector<int> faces, int idFace)
{
	std::vector<glm::vec3> vertexFace;

	vertexFace.push_back(glm::vec3(vertex[faces[idFace    ]], vertex[faces[idFace    ] + 1], vertex[faces[idFace    ] + 2]));
	vertexFace.push_back(glm::vec3(vertex[faces[idFace + 1]], vertex[faces[idFace + 1] + 1], vertex[faces[idFace + 1] + 2]));
	vertexFace.push_back(glm::vec3(vertex[faces[idFace + 2]], vertex[faces[idFace + 2] + 1], vertex[faces[idFace + 2] + 2]));

	return vertexFace;
}

std::vector<int> GetAdjacentFaces(std::vector<int> faces, int idFace)
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

std::vector<int> GetAdjacentVertex(std::vector<int> faces, int idFace, int idVertex)
{
	std::vector<int> adjFaces = GetAdjacentFaces(faces, idFace);
	std::vector<int> adjVertex;

	for (int i = 0; i < adjFaces.size(); i ++)
	{
		std::vector<int> vertex = GetIdVertexInFaces(faces, adjFaces[i]);

		for (int j = 0; j < vertex.size(); j++)
		{
			if (vertex[j] != idVertex)
			{
				int nbFound = 0;
				for (int k = 0; k < adjVertex.size(); k++)
				{
					if (vertex[j] == adjVertex[k])
						nbFound++;
				}

				if (nbFound == 0)
					adjVertex.push_back(vertex[j]);
			}
		}
	}

	return adjVertex;
}

std::vector<float> GetPertubedVertexKobbelt(std::vector<float> vertex, std::vector<int> faces)
{
	std::vector<float> newVertex = vertex;
	std::vector<bool> vertexAlreadyPertubed;

	for (int i = 0; i < faces.size() - 2; i += 3)
	{
		vertexAlreadyPertubed.push_back(false);
	}

	for (int i = 0; i < faces.size(); i++)
	{
		if (vertexAlreadyPertubed[faces[i]] == false)
		{
			std::vector<int> adjVertex = GetAdjacentVertex(faces, i / 3, faces[i]);

			glm::vec3 newPerVertex = (1 - Perturbation(adjVertex.size())) * 
				glm::vec3(vertex[faces[i]], vertex[faces[i] + 1], vertex[faces[i] + 2]);

			glm::vec3 somme(0, 0, 0);
			for (int i = 0; i < adjVertex.size(); i++)
				somme += glm::vec3(vertex[adjVertex[i]], vertex[adjVertex[i] + 1], vertex[adjVertex[i] + 2]);

			newPerVertex = newPerVertex + (Perturbation(adjVertex.size()) / adjVertex.size()) * somme;

			newVertex[i] = newPerVertex.x;
			newVertex[i + 1] = newPerVertex.y;
			newVertex[i + 2] = newPerVertex.z;
		}
	}

	return newVertex;
}

float Perturbation(int n)
{
	return (1 / 9.0f) * (4 - 2 * cos(2 * M_PI / n));
}

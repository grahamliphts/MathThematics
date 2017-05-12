#include "Subdivision.h"

void SubdivisionKobbelt(std::vector<float> &vertex, std::vector<int> &faces, int iteration)
{
	for (int iter = 0; iter < iteration; iter++)
	{
		std::vector<float> newVertex = GetPertubedVertexKobbelt(vertex, faces);
		std::vector<float> newAllVertex;
		std::vector<int> newFaces;

		std::vector<int> vertexCenterAlreadyAdd;
		std::vector<int> vertexCenterAlreadyid;
		std::vector<int> vertexAlreadyAdd;

		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
			vertexCenterAlreadyAdd.push_back(-1);
			vertexCenterAlreadyid.push_back(-1);
		}
		for (int iFace = 0; iFace < vertex.size() / 3; iFace ++)
			vertexAlreadyAdd.push_back(-1);

		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
			std::vector<int> idVertex;

			if (vertexCenterAlreadyAdd[iFace / 3] == -1)
			{
				std::vector<glm::vec3> vertexFace = GetVertexInFaces(vertex, faces, iFace / 3);
				glm::vec3 vertexCenter = (vertexFace[0] + vertexFace[1] + vertexFace[2]) / 3.0f;

				idVertex.push_back(newAllVertex.size() / 3);
				vertexCenterAlreadyAdd[iFace / 3] = iFace / 3;
				vertexCenterAlreadyid[iFace / 3] = newAllVertex.size() / 3;

				newAllVertex.push_back(vertexCenter.x);
				newAllVertex.push_back(vertexCenter.y);
				newAllVertex.push_back(vertexCenter.z);
			}
			else
				idVertex.push_back(vertexCenterAlreadyid[iFace / 3]);


			std::vector<int> adjFaces = GetAdjacentFaces(faces, vertexCenterAlreadyAdd[iFace / 3]);
			std::vector<glm::vec3> adjCenter;
			std::vector<int> idFaceCenter;
			std::vector<int> idFacesVertex;

			for (int i = 0; i < adjFaces.size(); i++)
			{
				if (vertexCenterAlreadyAdd[adjFaces[i]] == -1)
				{
					idVertex.push_back(newAllVertex.size() / 3);
					vertexCenterAlreadyAdd[adjFaces[i]] = adjFaces[i];
					vertexCenterAlreadyid[adjFaces[i]] = newAllVertex.size() / 3;

					std::vector<glm::vec3> vertexAdjFace = GetVertexInFaces(vertex, faces, adjFaces[i]);
					glm::vec3 vertexCenterAdj = (vertexAdjFace[0] + vertexAdjFace[1] + vertexAdjFace[2]) / 3.0f;

					newAllVertex.push_back(vertexCenterAdj.x);
					newAllVertex.push_back(vertexCenterAdj.y);
					newAllVertex.push_back(vertexCenterAdj.z);
				}
				else
					idVertex.push_back(vertexCenterAlreadyid[adjFaces[i]]);

				idFaceCenter.push_back(adjFaces[i]);
			}


			if (vertexAlreadyAdd[faces[iFace]] == -1)
			{
				idVertex.push_back(newAllVertex.size() / 3);
				vertexAlreadyAdd[faces[iFace]] = newAllVertex.size() / 3;

				newAllVertex.push_back(newVertex[faces[iFace] * 3]);
				newAllVertex.push_back(newVertex[faces[iFace] * 3 + 1]);
				newAllVertex.push_back(newVertex[faces[iFace] * 3 + 2]);
			}
			else
				idVertex.push_back(vertexAlreadyAdd[faces[iFace]]);

			idFacesVertex = GetFacesFromVertex(faces, faces[iFace], iFace / 3);
			for (int k = 0; k < idFacesVertex.size(); k++)
			{
				if (idFacesVertex[k] == idFaceCenter[0] || idFacesVertex[k] == idFaceCenter[1] || idFacesVertex[k] == idFaceCenter[2])
				{

					if (idFacesVertex[k] == idFaceCenter[0])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[1]);
						faceToFind.push_back(idVertex[4]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[1]);
							newFaces.push_back(idVertex[4]);
						}
					}
					else if (idFacesVertex[k] == idFaceCenter[1])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[2]);
						faceToFind.push_back(idVertex[4]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[2]);
							newFaces.push_back(idVertex[4]);
						}
					}
					else if (idFacesVertex[k] == idFaceCenter[2])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[3]);
						faceToFind.push_back(idVertex[4]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[3]);
							newFaces.push_back(idVertex[4]);
						}
					}
				}
			}

			if (vertexAlreadyAdd[faces[iFace + 1]] == -1)
			{
				idVertex.push_back(newAllVertex.size() / 3);
				vertexAlreadyAdd[faces[iFace + 1]] = newAllVertex.size() / 3;

				newAllVertex.push_back(newVertex[faces[iFace + 1] * 3]);
				newAllVertex.push_back(newVertex[faces[iFace + 1] * 3 + 1]);
				newAllVertex.push_back(newVertex[faces[iFace + 1] * 3 + 2]);
			}
			else
				idVertex.push_back(vertexAlreadyAdd[faces[iFace + 1]]);

			idFacesVertex = GetFacesFromVertex(faces, faces[iFace + 1], iFace / 3);
			for (int k = 0; k < idFacesVertex.size(); k++)
			{
				if (idFacesVertex[k] == idFaceCenter[0] || idFacesVertex[k] == idFaceCenter[1] || idFacesVertex[k] == idFaceCenter[2])
				{
					if (idFacesVertex[k] == idFaceCenter[0])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[1]);
						faceToFind.push_back(idVertex[5]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[1]);
							newFaces.push_back(idVertex[5]);
						}
					}
					else if (idFacesVertex[k] == idFaceCenter[1])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[2]);
						faceToFind.push_back(idVertex[5]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[2]);
							newFaces.push_back(idVertex[5]);
						}
					}
					else if (idFacesVertex[k] == idFaceCenter[2])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[3]);
						faceToFind.push_back(idVertex[5]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[3]);
							newFaces.push_back(idVertex[5]);
						}
					}
				}
			}

			if (vertexAlreadyAdd[faces[iFace + 2]] == -1)
			{
				idVertex.push_back(newAllVertex.size() / 3);
				vertexAlreadyAdd[faces[iFace + 2]] = newAllVertex.size() / 3;

				newAllVertex.push_back(newVertex[faces[iFace + 2] * 3]);
				newAllVertex.push_back(newVertex[faces[iFace + 2] * 3 + 1]);
				newAllVertex.push_back(newVertex[faces[iFace + 2] * 3 + 2]);
			}
			else
				idVertex.push_back(vertexAlreadyAdd[faces[iFace + 2]]);


			idFacesVertex = GetFacesFromVertex(faces, faces[iFace + 2], iFace / 3);
			for (int k = 0; k < idFacesVertex.size(); k++)
			{
				if (idFacesVertex[k] == idFaceCenter[0] || idFacesVertex[k] == idFaceCenter[1] || idFacesVertex[k] == idFaceCenter[2])
				{
					if (idFacesVertex[k] == idFaceCenter[0])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[1]);
						faceToFind.push_back(idVertex[6]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[1]);
							newFaces.push_back(idVertex[6]);
						}
					}
					else if (idFacesVertex[k] == idFaceCenter[1])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[2]);
						faceToFind.push_back(idVertex[6]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[2]);
							newFaces.push_back(idVertex[6]);
						}
					}
					else if (idFacesVertex[k] == idFaceCenter[2])
					{
						std::vector<int> faceToFind;
						faceToFind.push_back(idVertex[0]);
						faceToFind.push_back(idVertex[3]);
						faceToFind.push_back(idVertex[6]);

						if (!FaceExists(newFaces, faceToFind))
						{
							newFaces.push_back(idVertex[0]);
							newFaces.push_back(idVertex[3]);
							newFaces.push_back(idVertex[6]);
						}
					}
				}
			}
		}

		vertex = newAllVertex;
		faces = newFaces;
	}
}

void SubdivisionLoop(std::vector<float> &vertex, std::vector<int> &faces, int iteration)
{
	std::vector<int> vertexCenterAlreadyAdd;
	std::vector<int> vertexAlreadyAdd;
	std::vector<int> vertexCenterAlreadyid;

	for (int iter = 0; iter < iteration; iter++)
	{
		std::vector<float> newVertex;
		std::vector<int> newFaces;

		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
			vertexCenterAlreadyAdd.push_back(-1);
			vertexCenterAlreadyid.push_back(-1);
		}
		for (int iFace = 0; iFace < vertex.size() / 3; iFace++)
			vertexAlreadyAdd.push_back(-1);

		for (int iFace = 0; iFace < faces.size() - 2; iFace += 3)
		{
			std::vector<int> idVertex;

			if (vertexCenterAlreadyAdd[iFace / 3] == -1)
			{
				std::vector<glm::vec3> vertexFace = GetVertexInFaces(vertex, faces, iFace / 3);
				glm::vec3 vertex = vertexFace[0];

				std::vector<int> adjacentVertex = GetAdjacentVertex(faces, iFace / 3, faces[iFace]);

				glm::vec3 somme(0, 0, 0);
				for (int iAdjVertex = 0; iAdjVertex < adjacentVertex.size(); iAdjVertex++)
					somme += adjacentVertex[iAdjVertex];
				//glm::vec3 newVertex = (1 - Alpha(adjacentVertex.size() * vertex)) + Alpha(adjacentVertex.size()) * somme;

				//idVertex.push_back(newAllVertex.size() / 3);
				//vertexCenterAlreadyAdd[iFace / 3] = iFace / 3;
				//vertexCenterAlreadyid[iFace / 3] = newAllVertex.size() / 3;

				//newAllVertex.push_back(newVertex.x);
				//newAllVertex.push_back(newVertex.y);
				//newAllVertex.push_back(newVertex.z);
			}
		}
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

	vertexFace.push_back(glm::vec3(vertex[faces[idFace * 3] * 3], vertex[faces[idFace * 3] * 3 + 1], vertex[faces[idFace * 3] * 3 + 2]));
	vertexFace.push_back(glm::vec3(vertex[faces[idFace * 3 + 1] * 3], vertex[faces[idFace * 3 + 1] * 3 + 1], vertex[faces[idFace * 3 + 1] * 3 + 2]));
	vertexFace.push_back(glm::vec3(vertex[faces[idFace * 3 + 2] * 3], vertex[faces[idFace * 3 + 2] * 3 + 1], vertex[faces[idFace * 3 + 2] * 3 + 2]));

	return vertexFace;
}

std::vector<int> GetAdjacentFaces(std::vector<int> faces, int idFace)
{
	std::vector<int> adjFaces;

	for (int i = 0; i < faces.size() - 2; i += 3)
	{
		if (i / 3 == idFace)
			continue;

		int commonVertex = 0;

		if (faces[idFace * 3] == faces[i] || faces[idFace * 3] == faces[i + 1] || faces[idFace * 3] == faces[i + 2])
			commonVertex++;
		if (faces[idFace * 3 + 1] == faces[i] || faces[idFace * 3 + 1] == faces[i + 1] || faces[idFace * 3 + 1] == faces[i + 2])
			commonVertex++;
		if (faces[idFace * 3 + 2] == faces[i] || faces[idFace * 3 + 2] == faces[i + 1] || faces[idFace * 3 + 2] == faces[i + 2])
			commonVertex++;

		if (commonVertex == 2)
			adjFaces.push_back(i / 3);
	}

	return adjFaces;
}

std::vector<int> GetAdjacentVertex(std::vector<int> faces, int idFace, int idVertex)
{
	std::vector<int> adjVertex;

	for (int i = 0; i < faces.size() - 2; i += 3)
	{
		if (i / 3 == idFace)
			continue;

		if (faces[i] == idVertex || faces[i + 1] == idVertex || faces[i + 2] == idVertex)
		{
			for (int j = 0; j < 3; j++)
			{
				if (faces[i + j] != idVertex)
				{
					int nbFound = 0;
					for (int k = 0; k < adjVertex.size(); k++)
					{
						if (faces[i + j] == adjVertex[k])
							nbFound++;
					}

					if (nbFound == 0)
						adjVertex.push_back(faces[i + j]);
				}
			}
		}
	}
	return adjVertex;
}

std::vector<float> GetPertubedVertexKobbelt(std::vector<float> vertex, std::vector<int> faces)
{
	std::vector<float> newVertex = vertex;
	std::vector<bool> vertexAlreadyPertubed;

	for (int i = 0; i < vertex.size() - 2; i += 3)
	{
		vertexAlreadyPertubed.push_back(false);
	}

	for (int i = 0; i < faces.size(); i++)
	{
		if (vertexAlreadyPertubed[faces[i]] == false)
		{
			std::vector<int> adjVertex = GetAdjacentVertex(faces, i / 3, faces[i]);

			glm::vec3 newPerVertex = (1 - Perturbation(adjVertex.size())) * 
				glm::vec3(vertex[faces[i] * 3], vertex[faces[i] * 3 + 1], vertex[faces[i] * 3 + 2]);

			glm::vec3 somme(0, 0, 0);
			for (int j = 0; j < adjVertex.size(); j++)
				somme += glm::vec3(vertex[adjVertex[j] * 3], vertex[adjVertex[j] * 3 + 1], vertex[adjVertex[j] * 3 + 2]);

			newPerVertex = newPerVertex + (Perturbation(adjVertex.size()) / adjVertex.size()) * somme;

			newVertex[faces[i] * 3] = newPerVertex.x;
			newVertex[faces[i] * 3 + 1] = newPerVertex.y;
			newVertex[faces[i] * 3 + 2] = newPerVertex.z;

			vertexAlreadyPertubed[faces[i]] = true;
		}
	}

	return newVertex;
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

	return alpha;
}

std::vector<int> GetFacesFromVertex(std::vector<int> faces, int idVertex, int faceIgnore = -1)
{
	std::vector<int> listFaces;

	for (int i = 0; i < faces.size() - 2; i += 3)
	{
		if (faceIgnore != -1 && faceIgnore == i / 3)
			continue;

		if (idVertex == faces[i] || idVertex == faces[i + 1] || idVertex == faces[i + 2])
			listFaces.push_back(i / 3);
	}

	return listFaces;
}

bool FaceExists(std::vector<int> faces, std::vector<int> faceToFind)
{
	bool find = false;

	if (faces.size() == 0)
		return false;

	for (int i = 0; i < faces.size() - 2; i += 3)
	{
		int commonVertex = 0;

		if (faces[i] == faceToFind[0] || faces[i] == faceToFind[1] || faces[i] == faceToFind[2])
			commonVertex++;
		if (faces[i + 1] == faceToFind[0] || faces[i + 1] == faceToFind[1] || faces[i + 1] == faceToFind[2])
			commonVertex++;
		if (faces[i + 2] == faceToFind[0] || faces[i + 2] == faceToFind[1] || faces[i + 2] == faceToFind[2])
			commonVertex++;

		if (commonVertex == 3)
		{
			find = true;
			break;
		}
	}

	return find;
}
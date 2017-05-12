#pragma once

#define _USE_MATH_DEFINES

#include <glm\glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>


void SubdivisionKobbelt(std::vector<float> &vertex, std::vector<int> &faces, int iteration);
void SubdivisionLoop(std::vector<float> &vertex, std::vector<int> &faces, int iteration);

void SubdivisionKobbelt(std::vector<float> &vertex, std::vector<int> &faces, int iteration);

std::vector<float> GetPertubedVertexKobbelt(std::vector<float> vertex, std::vector<int> faces);
std::vector<int> GetIdVertexInFaces(std::vector<int> faces, int idFace);
std::vector<glm::vec3> GetVertexInFaces(std::vector<float> vertex, std::vector<int> faces, int idFace);
std::vector<int> GetAdjacentFaces(std::vector<int> faces, int idFace);
std::vector<int> GetAdjacentVertex(std::vector<int> faces, int idFace, int idVertex);
float Perturbation(int n);

std::vector<int> GetFacesFromVertex(std::vector<int> faces, int idVertex, int faceIgnore);
bool FaceExists(std::vector<int> faces, std::vector<int> faceToFind);
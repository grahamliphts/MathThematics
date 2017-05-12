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

std::vector<glm::vec3> getVertexInFaces(std::vector<float> vertex, std::vector<int> faces, int idFace);
std::vector<int> getAdjacentFaces(std::vector<int> faces, int idFace);
float Perturbation(int n);
float Alpha(int n);

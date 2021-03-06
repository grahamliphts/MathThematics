#pragma once

#include <glm\glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

std::vector<glm::vec3> GetChaikinCurve(std::vector<glm::vec3> controlPoints, int iter, float lowerRatio, float highRatio);
std::vector<std::vector<glm::vec3>> GetChaikinCurves(std::vector<std::vector<glm::vec3>> controlPoints, int iter, float lowerRatio, float highRatio);

std::vector<std::vector<glm::vec3>> CoonsPatch(std::vector<std::vector<glm::vec3>> controlPoints);
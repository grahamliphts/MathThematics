#include "ChaikinConnsFunctions.h"

std::vector<glm::vec3> GetChaikinCurve(std::vector<glm::vec3> controlPoints, int iter)
{
	std::vector<glm::vec3> chaikinCurve;
	
	if (iter < 1 && controlPoints.size() < 3)
		return chaikinCurve;


	for (int j = 1; j <= iter; j++)
	{
		chaikinCurve.push_back(controlPoints[0]);
		for (int i = 0; i < controlPoints.size() - 1; i++)
		{
			glm::vec3 pointA = controlPoints[i];
			glm::vec3 pointB = controlPoints[i + 1];

			if(i != 0)
				chaikinCurve.push_back(pointA + (pointB - pointA) * 0.25f);

			if(i + 1 != controlPoints.size() - 1)
				chaikinCurve.push_back(pointA + (pointB - pointA) * 0.75f);
		}
		chaikinCurve.push_back(controlPoints[controlPoints.size() - 1]);

		if (j != iter)
		{
			controlPoints = chaikinCurve;
			chaikinCurve.clear();
		}
	}

	return chaikinCurve;
}

std::vector<std::vector<glm::vec3>> GetChaikinCurves(std::vector<std::vector<glm::vec3>> controlPoints, int iter)
{
	std::vector<std::vector<glm::vec3>> chaikinCurves;

	for (int i = 0; i < controlPoints.size(); i++)
	{
		chaikinCurves.push_back(GetChaikinCurve(controlPoints[i], iter));
	}

	return chaikinCurves;
}

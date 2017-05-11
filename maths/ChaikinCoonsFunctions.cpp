#include "ChaikinConnsFunctions.h"

std::vector<glm::vec3> GetChaikinCurve(std::vector<glm::vec3> controlPoints, int iter, float lowerRatio, float highRatio)
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
				chaikinCurve.push_back(pointA + (pointB - pointA) * lowerRatio);

			if(i + 1 != controlPoints.size() - 1)
				chaikinCurve.push_back(pointA + (pointB - pointA) * highRatio);
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

std::vector<std::vector<glm::vec3>> GetChaikinCurves(std::vector<std::vector<glm::vec3>> controlPoints, int iter, float lowerRatio, float highRatio)
{
	std::vector<std::vector<glm::vec3>> chaikinCurves;

	for (int i = 0; i < controlPoints.size(); i++)
	{
		chaikinCurves.push_back(GetChaikinCurve(controlPoints[i], iter, lowerRatio, highRatio));
	}

	return chaikinCurves;
}

std::vector<std::vector<glm::vec3>> CoonsPatch(std::vector<std::vector<glm::vec3>> controlPoints)
{
	std::vector<std::vector<glm::vec3>> patchPoints;

	if (controlPoints.size() == 4)
	{
		patchPoints.push_back(controlPoints[0]);

		for (int j = 1; j < controlPoints[0].size() - 1; j++)
		{
			std::vector<glm::vec3> curvePatch;
			for (int i = 0; i < controlPoints[0].size(); i++)
			{
				float x, y, z;
				int nbPoints = controlPoints[0].size() - 1;


				float x1, x2, xb;

				x1 = (1 - (j * 1.0f / nbPoints)) * controlPoints[0][i].x + (j * 1.0f / nbPoints) * controlPoints[1][i].x;
				x2 = (1 - (i * 1.0f / nbPoints)) * controlPoints[2][j].x + (i * 1.0f / nbPoints) * controlPoints[3][j].x;
				xb = controlPoints[0][0].x * (1 - (i * 1.0f / nbPoints)) * (1 - (j * 1.0f / nbPoints))
					+ controlPoints[0][nbPoints].x * (i * 1.0f / nbPoints) * (1 - (j * 1.0f / nbPoints))
					+ controlPoints[1][0].x * (1 - (i * 1.0f / nbPoints)) * (j * 1.0f / nbPoints)
					+ controlPoints[1][nbPoints].x * (i * 1.0f / nbPoints) * (j * 1.0f / nbPoints);

				x = x1 + x2 - xb;

				//x = controlPoints[0][i].x + (controlPoints[1][i].x - controlPoints[0][i].x) * (i * 1.0f / nbPoints);
				//z = controlPoints[2][j].z + (controlPoints[3][j].z - controlPoints[2][j].z) * (j * 1.0f / nbPoints);


				float y1, y2, yb;

				y1 = (1 - (j * 1.0f / nbPoints)) * controlPoints[0][i].y + (j * 1.0f / nbPoints) * controlPoints[1][i].y;
				y2 = (1 - (i * 1.0f / nbPoints)) * controlPoints[2][j].y + (i * 1.0f / nbPoints) * controlPoints[3][j].y;
				yb = controlPoints[0][0].y * (1 - (i * 1.0f / nbPoints)) * (1 - (j * 1.0f / nbPoints))
					+ controlPoints[0][nbPoints].y * (i * 1.0f / nbPoints) * (1 - (j * 1.0f / nbPoints))
					+ controlPoints[1][0].y * (1 - (i * 1.0f / nbPoints)) * (j * 1.0f / nbPoints)
					+ controlPoints[1][nbPoints].y * (i * 1.0f / nbPoints) * (j * 1.0f / nbPoints);

				//B(s,t)=c_{0}(0)(1-s)(1-t)+c_{0}(1)s(1-t)+c_{1}(0)(1-s)t+c_{1}(1)st

				y = y1 + y2 - yb;

				float z1, z2, zb;

				z1 = (1 - (j * 1.0f / nbPoints)) * controlPoints[0][i].z + (j * 1.0f / nbPoints) * controlPoints[1][i].z;
				z2 = (1 - (i * 1.0f / nbPoints)) * controlPoints[2][j].z + (i * 1.0f / nbPoints) * controlPoints[3][j].z;
				zb = controlPoints[0][0].z * (1 - (i * 1.0f / nbPoints)) * (1 - (j * 1.0f / nbPoints))
					+ controlPoints[0][nbPoints].y * (i * 1.0f / nbPoints) * (1 - (j * 1.0f / nbPoints))
					+ controlPoints[1][0].z * (1 - (i * 1.0f / nbPoints)) * (j * 1.0f / nbPoints)
					+ controlPoints[1][nbPoints].z * (i * 1.0f / nbPoints) * (j * 1.0f / nbPoints);


				z = z1 + z2 - zb;

				curvePatch.push_back(glm::vec3(x, y, z));
			}

			patchPoints.push_back(curvePatch);
		}

		patchPoints.push_back(controlPoints[1]);
	}

	return patchPoints;
}

// --------------------------------------------------
// Projet Mathématiques
// 5ème Année 3DJV
// 
// Courbes de Chaikin, Enveloppe de Coons et Subdivision
// --------------------------------------------------

// --------------------------------------------------
// INCLUDES
// --------------------------------------------------
#pragma region Include Library
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include "EsgiShader.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <math.h>
#pragma endregion

#pragma region Include Files
#include "Camera.h"
#include "ChaikinConnsFunctions.h"
#include "Subdivision.h"
#include "Cube.h"
#pragma endregion
// --------------------------------------------------

using namespace glm;

// --------------------------------------------------
// VARIABLES GLOBALES
// --------------------------------------------------
#pragma region Variables Globales
Camera *_cam;
Cube *_cube;
GLint _gridProgram, _basicProgram, _basicColorProgram;
int _width = 1500;
int _height = 800;

float _fragmentColor[4];

int _mode = 0;

//Parameters for Original Curves
ImVec4 _originalCurveColor = ImColor(172, 255, 143);
int _nbPoints = 3;
int _limitsXZ = 5;
int _limitsY = 3;

int _current_curve = 1;
int _current_point = 1;

//Parameters for Chaikin
ImVec4 _chaikinCurveColor = ImColor(114, 0, 6);
float _lowerRatio = 0.25f;
float _highRatio = 0.75f;
int _iterationsChaikin = 2;
bool _showPointChaikin = true;

//Parameters for Coons
ImVec4 _coonsColor = ImColor(4, 33, 144);
bool _showPointCoons = true;

//Parameters for Subdivision
int _iterationsSubdivision = 1;
ImVec4 _cubeColor = ImColor(90, 33, 144);

GLuint _vaoChaikinCurves, _vaoOriginalCurves, _vaoCoons;
GLuint _vertexBufferChaikinCurves, _vertexBufferOriginalCurves, _vertexBufferCoons, _vertexBufferOriginalCurvesColor;

std::vector<std::vector<glm::vec3>> _chaikinCurves;
std::vector<std::vector<glm::vec3>> _originalCurves;
struct Color
{
	float r, g, b, a;
	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
};
std::vector<std::vector<Color>> _colors;

std::vector<std::vector<glm::vec3>> _coonsPatch;
#pragma endregion
#pragma region Structures
struct
{
	struct
	{
		int     model_matrix;
		int     projview_matrix;
		int     position;
		int		color;
	} basic;
	struct
	{
		int     model_matrix;
		int     projview_matrix;
		int     position;
		int     color;
	} basicColor;
	struct
	{
		int     projview_matrix;
	} grid;
} uniforms;
#pragma endregion
// --------------------------------------------------

// --------------------------------------------------
// PROTOTYPES
// --------------------------------------------------
#pragma region Prototypes Fonctions
void Initialize();
void Render(void);
void LoadShaders();
static void ErrorCallback(int error, const char* description);

void CallbackMousePos(GLFWwindow *window, int button, int action, int mods);
void CallbackMouseMove(GLFWwindow *window, double x, double y);
void CallbackKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

void CreateControlCurves();
void DrawUI();

template<typename T>
void MajBuffer(int vertexBuffer, std::vector<T> &vecteur);
void SetColorToFragment(ImVec4 &color);
void GetCurveAndPointsAdj(int current_curve, int current_point, int &curveAdj, int &pointAdj);
#pragma endregion
// --------------------------------------------------

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main(int argc, char** argv)
{
	

    // Setup window
    glfwSetErrorCallback(ErrorCallback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    GLFWwindow* window = glfwCreateWindow(_width, _height, "Coons & Subvision", NULL, NULL);
    glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	gl3wInit();
	
    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

	glfwSetMouseButtonCallback(window, CallbackMousePos);
	glfwSetCursorPosCallback(window, CallbackMouseMove);
	glfwSetKeyCallback(window, CallbackKeyboard);

	LoadShaders();
	Initialize();


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
		DrawUI();

		_cam->updatePos();

        // Rendering
        glViewport(0, 0, _width, _height);
		glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPointSize(10);

		Render();
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
// --------------------------------------------------


// --------------------------------------------------
// INITIALISATION
// --------------------------------------------------
#pragma region Initialisation et Shader
void Initialize()
{
	/*VAO Chaikin Curve*/
	glGenVertexArrays(1, &_vaoChaikinCurves);
	glBindVertexArray(_vaoChaikinCurves);

	glGenBuffers(1, &_vertexBufferChaikinCurves);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferChaikinCurves);
	
	glEnableVertexAttribArray(uniforms.basic.position);
	glVertexAttribPointer(uniforms.basic.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	/*VAO Original Curve*/
	glGenVertexArrays(1, &_vaoOriginalCurves);
	glBindVertexArray(_vaoOriginalCurves);

	glGenBuffers(1, &_vertexBufferOriginalCurves);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferOriginalCurves);

	glEnableVertexAttribArray(uniforms.basicColor.position);
	glVertexAttribPointer(uniforms.basicColor.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &_vertexBufferOriginalCurvesColor);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferOriginalCurvesColor);

	glEnableVertexAttribArray(uniforms.basicColor.color);
	glVertexAttribPointer(uniforms.basicColor.color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	/*VAO Original Coons*/
	glGenVertexArrays(1, &_vaoCoons);
	glBindVertexArray(_vaoCoons);

	glGenBuffers(1, &_vertexBufferCoons);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferCoons);

	glEnableVertexAttribArray(uniforms.basic.position);
	glVertexAttribPointer(uniforms.basic.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	/*Create Camera*/
	_cam = new Camera();

	/*Create Cube*/
	_cube = new Cube();
	_cube->initialize(uniforms.basic.position);
}

void LoadShaders()
{
	EsgiShader gridShader, basicShader, basicColorShader;

	printf("Load basic fs\n");
	basicShader.LoadFragmentShader("shaders/basic.frag");
	printf("Load basic vs\n");
	basicShader.LoadVertexShader("shaders/basic.vert");
	basicShader.Create();

	printf("Load basic color fs\n");
	basicColorShader.LoadFragmentShader("shaders/basicColor.frag");
	printf("Load basic color vs\n");
	basicColorShader.LoadVertexShader("shaders/basicColor.vert");
	basicColorShader.Create();

	printf("Load grid fs\n");
	gridShader.LoadFragmentShader("shaders/grid.frag");
	printf("Load grid vs\n");
	gridShader.LoadVertexShader("shaders/grid.vert");
	printf("Load grid geom\n");
	gridShader.LoadGeometryShader("shaders/grid.geom");
	gridShader.Create();

	_gridProgram = gridShader.GetProgram();
	_basicProgram = basicShader.GetProgram();
	_basicColorProgram = basicColorShader.GetProgram();

	uniforms.grid.projview_matrix = glGetUniformLocation(_gridProgram, "projview_matrix");

	uniforms.basic.projview_matrix = glGetUniformLocation(_basicProgram, "projview_matrix");
	uniforms.basic.model_matrix = glGetUniformLocation(_basicProgram, "model_matrix");
	uniforms.basic.position = glGetAttribLocation(_basicProgram, "a_position");
	uniforms.basic.color = glGetUniformLocation(_basicProgram, "fragmentColor");

	uniforms.basicColor.projview_matrix = glGetUniformLocation(_basicColorProgram, "projview_matrix");
	uniforms.basicColor.model_matrix = glGetUniformLocation(_basicColorProgram, "model_matrix");
	uniforms.basicColor.position = glGetAttribLocation(_basicColorProgram, "a_position");
	uniforms.basicColor.color = glGetAttribLocation(_basicColorProgram, "a_color");
}
#pragma endregion
// --------------------------------------------------


// --------------------------------------------------
// RENDER
// --------------------------------------------------
void Render()
{
	//Parameters
	glm::vec3 cam_pos = glm::vec3(_cam->posx, _cam->posy, _cam->posz);
	glm::mat4 proj = glm::perspective(45.0f, (float)_width / _height, 0.01f, 1000.0f);
	glm::mat4 view = _cam->GetOrientation() * glm::translate(cam_pos);
	glm::mat4 proj_view = proj * view;
	glm::mat4 model_mat;

	//Draw axes
	glUseProgram(_gridProgram);
	glUniformMatrix4fv(uniforms.grid.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
	glDrawArrays(GL_POINTS, 0, 1);


	if (_mode == 0)
	{
		//Draw original curves
		glUseProgram(_basicColorProgram);
		glUniformMatrix4fv(uniforms.basicColor.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);

		model_mat = glm::translate(glm::vec3(-2.5, 0, -2.5));
		glUniformMatrix4fv(uniforms.basicColor.model_matrix, 1, GL_FALSE, (GLfloat*)&model_mat[0][0]);


		glBindVertexArray(_vaoOriginalCurves);
		if (_originalCurves.size() > 0)
		{
			int curveAdj = -1, pointAdj = -1;
			GetCurveAndPointsAdj(_current_curve - 1, _current_point - 1, curveAdj, pointAdj);
			for (int i = 0; i < _colors.size(); i++)
			{
				for (int j = 0; j < _colors[i].size(); j++)
				{
					if ((i == _current_curve - 1 && j == _current_point - 1) || (i == curveAdj && j == pointAdj))
					{
						_colors[i][j].r = 1.0f;
						_colors[i][j].b = 0.0f;
						_colors[i][j].g = 0.0f;
					}
					else
					{
						_colors[i][j].r = 0.0f;
						_colors[i][j].b = 0.0f;
						_colors[i][j].g = 0.0f;
					}
				}
			}

			for (int i = 0; i < 4; i++)
			{

				MajBuffer(_vertexBufferOriginalCurvesColor, _colors[i]);
				MajBuffer(_vertexBufferOriginalCurves, _originalCurves[i]);
				glDrawArrays(GL_POINTS, 0, _originalCurves[i].size());
				glDrawArrays(GL_LINE_STRIP, 0, _originalCurves[i].size());
			}
		}
		glBindVertexArray(0);
		glUseProgram(0);

		//Draw Chaikin curves
		glUseProgram(_basicProgram);
		glUniformMatrix4fv(uniforms.basic.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
		glUniformMatrix4fv(uniforms.basic.model_matrix, 1, GL_FALSE, (GLfloat*)&model_mat[0][0]);

		glBindVertexArray(_vaoChaikinCurves);
		if (_chaikinCurves.size() > 0)
		{
			SetColorToFragment(_chaikinCurveColor);
			for (int i = 0; i < 4; i++)
			{
				MajBuffer(_vertexBufferChaikinCurves, _chaikinCurves[i]);
				if (_showPointChaikin)
					glDrawArrays(GL_POINTS, 0, _chaikinCurves[i].size());
				glDrawArrays(GL_LINE_STRIP, 0, _chaikinCurves[i].size());
			}
		}
		glBindVertexArray(0);

		//Draw Chaikin coons
		glBindVertexArray(_vaoCoons);
		if (_chaikinCurves.size() == 4 && _coonsPatch.size() > 0)
		{
			SetColorToFragment(_coonsColor);
			for (int i = 0; i < _chaikinCurves[0].size(); i++)
			{
				MajBuffer(_vertexBufferCoons, _coonsPatch[i]);
				glDrawArrays(GL_LINE_STRIP, 0, _coonsPatch[i].size());
			}

			for (int j = 0; j < _coonsPatch.size(); j++)
			{
				std::vector<glm::vec3> curvePatch;
				for (int i = 0; i < _coonsPatch.size(); i++)
				{
					curvePatch.push_back(_coonsPatch[i][j]);
				}
				MajBuffer(_vertexBufferCoons, curvePatch);
				if (_showPointCoons)
					glDrawArrays(GL_POINTS, 0, curvePatch.size());
				glDrawArrays(GL_LINE_STRIP, 0, curvePatch.size());
			}
		}
		glBindVertexArray(0);
		glUseProgram(0);

	}
	else
	{
		glUseProgram(_basicProgram);
		glUniformMatrix4fv(uniforms.basic.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
		glUniformMatrix4fv(uniforms.basic.model_matrix, 1, GL_FALSE, (GLfloat*)&model_mat[0][0]);

		SetColorToFragment(_cubeColor);
		_cube->draw();
		glUseProgram(0);
	}
}

// --------------------------------------------------



// --------------------------------------------------
// CALLBACK
// --------------------------------------------------
static void ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void CallbackMousePos(GLFWwindow *window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_RELEASE)
			_cam->releaseCam();
		else
			_cam->grabCam(x, y);
	}
}

void CallbackMouseMove(GLFWwindow *window, double x, double y)
{
	_cam->orienterCam(x, y);
}

void CallbackKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_Z:
			_cam->deltaForward = -1;
			break;
		case GLFW_KEY_S:
			_cam->deltaForward = 1;
			break;
		case GLFW_KEY_Q:
			_cam->deltaStrafe = -1;
			break;
		case GLFW_KEY_D:
			_cam->deltaStrafe = 1;
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_Z:
		case GLFW_KEY_S:
			_cam->deltaForward = 0;
			break;
		case GLFW_KEY_Q:
		case GLFW_KEY_D:
			_cam->deltaStrafe = 0;
			break;
		}
	}
}

void CreateControlCurves()
{
	_originalCurves.clear();
	_colors.clear();

	std::vector<glm::vec3> originalCurve;
	std::vector<Color> color;

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(0, 0, 0));
	color.push_back(Color());
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3((i * 1.f / (_nbPoints - 1) * _limitsXZ), randY, 0));
		color.push_back(Color());
	}
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, 0));
	color.push_back(Color());

	_originalCurves.push_back(originalCurve);
	_colors.push_back(color);

	originalCurve.clear();
	color.clear();

	// ----------------------------------------

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(0, 0, _limitsXZ));
	color.push_back(Color());
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3((i * 1.f / (_nbPoints - 1) * _limitsXZ), randY, _limitsXZ));
		color.push_back(Color());
	}
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, _limitsXZ));
	color.push_back(Color());

	_originalCurves.push_back(originalCurve);
	_colors.push_back(color);

	originalCurve.clear();
	color.clear();
	// ----------------------------------------

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(0, 0, 0));
	color.push_back(Color());
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3(0, randY, (i * 1.f / (_nbPoints - 1) * _limitsXZ)));
		color.push_back(Color());
	}
	originalCurve.push_back(glm::vec3(0, 0, _limitsXZ));
	color.push_back(Color());

	_originalCurves.push_back(originalCurve);
	_colors.push_back(color);

	originalCurve.clear();
	color.clear();

	// ----------------------------------------

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, 0));
	color.push_back(Color());
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3(_limitsXZ, randY, (i * 1.f / (_nbPoints - 1) * _limitsXZ)));
		color.push_back(Color());
	}
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, _limitsXZ));
	color.push_back(Color());

	_originalCurves.push_back(originalCurve);
	_colors.push_back(color);

	originalCurve.clear();
	color.clear();

	MajBuffer(_vertexBufferOriginalCurvesColor, _colors);
	// ----------------------------------------
}
// --------------------------------------------------


template<typename T>
void MajBuffer(int vertexBuffer, std::vector<T> &vecteur)
{
	if (vecteur.size() <= 0)
		return;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vecteur.size(), vecteur.data(), GL_STATIC_DRAW);
}

void SetColorToFragment(ImVec4 &color)
{
	_fragmentColor[0] = color.x;
	_fragmentColor[1] = color.y;
	_fragmentColor[2] = color.z;
	_fragmentColor[3] = color.w;
	glProgramUniform4fv(_basicProgram, uniforms.basic.color, 1, _fragmentColor);
}

void GetCurveAndPointsAdj(int current_curve, int current_point, int &curveAdj, int &pointAdj)
{
	if (current_curve == 0 && current_point == _colors[0].size() - 1)
	{
		curveAdj = 3;
		pointAdj = 0;
	}
	else if (current_curve == 1 && current_point == _colors[0].size() - 1)
	{
		curveAdj = 3;
		pointAdj = _colors[0].size() - 1;
	}
	else if (current_curve == 2 && current_point == _colors[0].size() - 1)
	{
		curveAdj = 1;
		pointAdj = 0;
	}
	else if (current_curve == 3 && current_point == _colors[0].size() - 1)
	{
		curveAdj = 1;
		pointAdj = _colors[0].size() - 1;
	}
	else if (current_curve == 0 && current_point == 0)
	{
		curveAdj = 2;
		pointAdj = 0;
	}
	else if (current_curve == 1 && current_point == 0)
	{
		curveAdj = 2;
		pointAdj = _colors[0].size() - 1;
	}
	else if (current_curve == 2 && current_point == 0)
	{
		curveAdj = 0;
		pointAdj = 0;
	}
	else if (current_curve == 3 && current_point == 0)
	{
		curveAdj = 0;
		pointAdj = _colors[0].size() - 1;
	}
}

void DrawUI()
{
	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Begin("Chaikin Curve / Coons / Subdivision");

	ImGui::RadioButton("Mode coons", &_mode, 0); ImGui::SameLine();
	ImGui::RadioButton("Mode subdivision", &_mode, 1);

	if (_mode == 0)
	{
		ImGui::Text("Parameters for Curves");
		ImGui::ColorEdit3("Original curve color", (float*)&_originalCurveColor);
		ImGui::SliderInt("Number Points", &_nbPoints, 3, 20);
		ImGui::SliderInt("Limits XY", &_limitsXZ, 1, 20);
		ImGui::SliderInt("Limit Z (+/-)", &_limitsY, 1, 10);

		if (ImGui::Button("Create New Curves"))
		{
			CreateControlCurves();
			_chaikinCurves = GetChaikinCurves(_originalCurves, _iterationsChaikin, _lowerRatio, _highRatio);
			_coonsPatch = CoonsPatch(_chaikinCurves);
		}

		if (_originalCurves.size() > 0)
		{
			if (ImGui::InputInt("Current curve", &_current_curve, 1))
			{
				_current_curve = _current_curve < 1 ? 1 : _current_curve;
				_current_curve = _current_curve > 4 ? 4 : _current_curve;
			}
			if (ImGui::InputInt("Current point", &_current_point, 1))
			{
				_current_point = _current_point < 1 ? 1 : _current_point;
				if (_originalCurves.size() > 0)
					_current_point = _current_point > _originalCurves[0].size() ? _originalCurves[0].size() : _current_point;
			}

			int xyzPoint[4] = { _originalCurves[_current_curve - 1][_current_point - 1].x, _originalCurves[_current_curve - 1][_current_point - 1].y, _originalCurves[_current_curve - 1][_current_point - 1].z, 255 };
			if (ImGui::SliderInt3("X Y Z", xyzPoint, -10, 10))
			{
				_originalCurves[_current_curve - 1][_current_point - 1].x = xyzPoint[0];
				_originalCurves[_current_curve - 1][_current_point - 1].y = xyzPoint[1];
				_originalCurves[_current_curve - 1][_current_point - 1].z = xyzPoint[2];

				int curveAdj = -1, pointAdj = -1;
				GetCurveAndPointsAdj(_current_curve - 1, _current_point - 1, curveAdj, pointAdj);
				if (curveAdj != -1 && pointAdj != -1)
				{
					_originalCurves[curveAdj][pointAdj].x = xyzPoint[0];
					_originalCurves[curveAdj][pointAdj].y = xyzPoint[1];
					_originalCurves[curveAdj][pointAdj].z = xyzPoint[2];
				}

				_chaikinCurves = GetChaikinCurves(_originalCurves, _iterationsChaikin, _lowerRatio, _highRatio);
				_coonsPatch = CoonsPatch(_chaikinCurves);
			}

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Text("Parameters for chaikin curve");
			ImGui::ColorEdit3("Chaikin curve color", (float*)&_chaikinCurveColor);
			if (ImGui::SliderInt("Iteration for chaikin curve", &_iterationsChaikin, 1, 5))
			{
				_chaikinCurves = GetChaikinCurves(_originalCurves, _iterationsChaikin, _lowerRatio, _highRatio);
				_coonsPatch = CoonsPatch(_chaikinCurves);
			}
			if (ImGui::DragFloatRange2("Ratio Corner Cutting", &_lowerRatio, &_highRatio, 0.001f, 0.1f, 0.9f))
			{
				_chaikinCurves = GetChaikinCurves(_originalCurves, _iterationsChaikin, _lowerRatio, _highRatio);
				_coonsPatch = CoonsPatch(_chaikinCurves);
			}
			ImGui::Checkbox("Show Points Chaikin", &_showPointChaikin);

			if (ImGui::Button("Update") && _originalCurves.size() != 0)
			{
				_chaikinCurves = GetChaikinCurves(_originalCurves, _iterationsChaikin, _lowerRatio, _highRatio);
				_coonsPatch = CoonsPatch(_chaikinCurves);
			}
			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Text("Parameters for coons");
			ImGui::ColorEdit3("Coons color", (float*)&_coonsColor);
			ImGui::Checkbox("Show Points Coons", &_showPointCoons);
		}
	}
	else
	{
		ImGui::Text("Parameters for subdivision");

		if (ImGui::Button("Subdivide Kobbelt"))
		{
			_cube->savePrevious();
			SubdivisionKobbelt(_cube->verts, _cube->faces, _iterationsSubdivision);
			_cube->initialize(uniforms.basic.position);
		} 
		ImGui::SameLine();
		if (ImGui::Button("Subdivide Loop"))
		{
			_cube->savePrevious();
			SubdivisionLoop(_cube->verts, _cube->faces, _iterationsSubdivision);
			_cube->initialize(uniforms.basic.position);
		}

		if (ImGui::Button("Undo"))
		{
			_cube->restorePrevious();
			_cube->initialize(uniforms.basic.position);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			_cube->reset();
			_cube->initialize(uniforms.basic.position);
		}

		if (ImGui::SliderInt("Iteration for subdivision", &_iterationsSubdivision, 1, 5));
		ImGui::ColorEdit3("Cube color", (float*)&_cubeColor);
	}
	//if (ImGui::Button("Create Patch") && _chaikinCurves.size() == 4)
	//	_coonsPatch = CoonsPatch(_chaikinCurves);

	ImGui::End();

	static bool p_open = true;
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	if (!ImGui::Begin("Example: Fixed Overlay", &p_open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("FPS Counter");
	ImGui::Separator();
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	//bool show_test_window = true;
	//ImGui::ShowTestWindow(&show_test_window);
}
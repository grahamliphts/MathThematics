// --------------------------------------------------
// Projet Math�matiques
// 5�me Ann�e 3DJV
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
#pragma endregion
// --------------------------------------------------

using namespace glm;

// --------------------------------------------------
// VARIABLES GLOBALES
// --------------------------------------------------
#pragma region Variables Globales
Camera *_cam;
GLint _gridProgram, _basicProgram;
int _width = 1500;
int _height = 800;

float _fragmentColor[4];

//Parameters for Original Curves
ImVec4 _originalCurveColor = ImColor(172, 255, 143);
int _nbPoints = 3;
int _limitsXZ = 5;
int _limitsY = 3;

//Parameters for Chaikin
ImVec4 _chaikinCurveColor = ImColor(114, 0, 6);
float _lowerRatio = 0.25f;
float _highRatio = 0.75f;
int _iterations = 2;
bool _showPointChaikin = true;

GLuint _vaoChaikinCurves, _vaoOriginalCurves;
GLuint _vertexBufferChaikinCurves, _vertexBufferOriginalCurves;

std::vector<std::vector<glm::vec3>> _chaikinCurves;
std::vector<std::vector<glm::vec3>> _originalCurves;
#pragma endregion

#pragma region Structures
struct
{
	struct
	{
		int     model_matrix;
		int     projview_matrix;
		int     position;
		int     color;
	} basic;
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

template<typename T>
void MajBuffer(int vertexBuffer, std::vector<T> &vecteur);
void SetColorToFragment(ImVec4 &color);
#pragma endregion
// --------------------------------------------------

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main(int argc, char** argv)
{
	_cam = new Camera();

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

    bool show_test_window = false;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Chaikin Curve / Coons / Subdivision");
		ImGui::Text("Parameters for Curves");
		ImGui::ColorEdit3("Original curve color", (float*)&_originalCurveColor);
		ImGui::SliderInt("Number Points", &_nbPoints, 3, 20);
		ImGui::SliderInt("Limits XY", &_limitsXZ, 1, 20);
		ImGui::SliderInt("Limit Z (+/-)", &_limitsY, 1, 10);

		if (ImGui::Button("Create New Curves"))
		{
			CreateControlCurves();
			_chaikinCurves = GetChaikinCurves(_originalCurves, _iterations, _lowerRatio, _highRatio);
		}

		ImGui::Spacing();
		ImGui::Separator();

		ImGui::Text("Parameters for chaikin curve");
		ImGui::ColorEdit3("Chaikin curve color", (float*)&_chaikinCurveColor);
		ImGui::SliderInt("Iteration for chaikin curve", &_iterations, 1, 5);
		ImGui::DragFloatRange2("Ratio Corner Cutting", &_lowerRatio, &_highRatio, 0.001f, 0.1f, 0.9f);
		ImGui::Checkbox("Show Points Chaikin", &_showPointChaikin);

		if (ImGui::Button("Update") && _originalCurves.size() != 0)
			_chaikinCurves = GetChaikinCurves(_originalCurves, _iterations, _lowerRatio, _highRatio);

		ImGui::Spacing();
		ImGui::Separator();

		ImGui::Text("Parameters for coons");

		ImGui::Spacing();
		ImGui::Separator();

        if (ImGui::Button("Test Window")) 
			show_test_window ^= 1;

		ImGui::End();

		static bool p_open = true;
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		if (!ImGui::Begin("Example: Fixed Overlay", &p_open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return 0;
		}
		ImGui::Text("FPS Counter");
		ImGui::Separator();
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

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
	/*VAO Points*/
	glGenVertexArrays(1, &_vaoChaikinCurves);
	glBindVertexArray(_vaoChaikinCurves);

	glGenBuffers(1, &_vertexBufferChaikinCurves);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferChaikinCurves);
	
	glEnableVertexAttribArray(uniforms.basic.position);
	glVertexAttribPointer(uniforms.basic.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	glGenVertexArrays(1, &_vaoOriginalCurves);
	glBindVertexArray(_vaoOriginalCurves);

	glGenBuffers(1, &_vertexBufferOriginalCurves);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferOriginalCurves);

	glEnableVertexAttribArray(uniforms.basic.position);
	glVertexAttribPointer(uniforms.basic.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

void LoadShaders()
{
	EsgiShader gridShader, basicShader;

	printf("Load basic fs\n");
	basicShader.LoadFragmentShader("shaders/basic.frag");
	printf("Load basic vs\n");
	basicShader.LoadVertexShader("shaders/basic.vert");
	basicShader.Create();

	printf("Load grid fs\n");
	gridShader.LoadFragmentShader("shaders/grid.frag");
	printf("Load grid vs\n");
	gridShader.LoadVertexShader("shaders/grid.vert");
	printf("Load grid geom\n");
	gridShader.LoadGeometryShader("shaders/grid.geom");
	gridShader.Create();

	_gridProgram = gridShader.GetProgram();
	_basicProgram = basicShader.GetProgram();

	uniforms.grid.projview_matrix = glGetUniformLocation(_gridProgram, "projview_matrix");

	uniforms.basic.projview_matrix = glGetUniformLocation(_basicProgram, "projview_matrix");
	uniforms.basic.model_matrix = glGetUniformLocation(_basicProgram, "model_matrix");
	uniforms.basic.position = glGetAttribLocation(_basicProgram, "a_position");
	uniforms.basic.color = glGetUniformLocation(_basicProgram, "fragmentColor");
}
#pragma endregion
// --------------------------------------------------


// --------------------------------------------------
// RENDER
// --------------------------------------------------
void Render(void)
{
	//Parameters
	glm::vec3 cam_pos = glm::vec3(_cam->posx, _cam->posy, _cam->posz);
	glm::mat4 proj = glm::perspective(45.0f, (float)_width / _height, 0.01f, 1000.0f);
	glm::mat4 view = _cam->GetOrientation() * glm::translate(cam_pos);
	glm::mat4 proj_view = proj * view;
	glm::mat4 model_mat;

	//Draw grid
	glUseProgram(_gridProgram);
	glUniformMatrix4fv(uniforms.grid.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
	glDrawArrays(GL_POINTS, 0, 1);

	//Draw Point and curves
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

	glBindVertexArray(_vaoOriginalCurves);
	if (_originalCurves.size() > 0)
	{
		SetColorToFragment(_originalCurveColor);
		for (int i = 0; i < 4; i++)
		{
			MajBuffer(_vertexBufferOriginalCurves, _originalCurves[i]);
			glDrawArrays(GL_LINE_STRIP, 0, _originalCurves[i].size());
		}
	}
	glBindVertexArray(0);

	glUseProgram(0);
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
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && ImGui::IsMouseHoveringAnyWindow() == 0)
	{
		std::cout << "Button mouse left press" << std::endl;
	}
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

	std::vector<glm::vec3> originalCurve;

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(0, 0, 0));
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3((i * 1.f / (_nbPoints - 1) * _limitsXZ), randY, 0));
	}
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, 0));
	_originalCurves.push_back(originalCurve);

	originalCurve.clear();
	// ----------------------------------------

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(0, 0, _limitsXZ));
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3((i * 1.f / (_nbPoints - 1) * _limitsXZ), randY, _limitsXZ));
	}
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, _limitsXZ));
	_originalCurves.push_back(originalCurve);

	originalCurve.clear();
	// ----------------------------------------

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(0, 0, 0));
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3(0, randY, (i * 1.f / (_nbPoints - 1) * _limitsXZ)));
	}
	originalCurve.push_back(glm::vec3(0, 0, _limitsXZ));
	_originalCurves.push_back(originalCurve);

	originalCurve.clear();
	// ----------------------------------------

	// ----------------------------------------
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, 0));
	for (int i = 1; i < _nbPoints - 1; i++)
	{
		float randY = (rand() % (_limitsY * 2 + 1) - _limitsY) * 0.1f;
		originalCurve.push_back(glm::vec3(_limitsXZ, randY, (i * 1.f / (_nbPoints - 1) * _limitsXZ)));
	}
	originalCurve.push_back(glm::vec3(_limitsXZ, 0, _limitsXZ));
	_originalCurves.push_back(originalCurve);

	originalCurve.clear();
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

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>

#include "EsgiShader.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "Camera.h"
#include "ChaikinConnsFunctions.h"

Camera *cam;
GLint gridProgram, basicProgram;
int width = 1500;
int height = 800;

float fragmentColor[4];

//Parameters for Chaikin
ImVec4 chaikinCurveColor = ImColor(114, 0, 6);
ImVec4 originalCurveColor = ImColor(172, 255, 143);
int iterations = 2;

GLuint vaoPoint;
GLuint vertexBufferPoints;

std::vector<glm::vec3> chaikinCurve;
std::vector<glm::vec3> originalCurve;

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

using namespace glm;

template<typename T>
void majBuffer(int vertexBuffer, std::vector<T> &vecteur);
void setColorToFragment(ImVec4 &color);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

void render(void)
{
	//Parameters
	glm::vec3 cam_pos = glm::vec3(cam->posx, cam->posy, cam->posz);
	glm::mat4 proj = glm::perspective(45.0f, (float)width / height, 0.01f, 1000.0f);
	glm::mat4 view = cam->GetOrientation() * glm::translate(cam_pos);
	glm::mat4 proj_view = proj * view;
	glm::mat4 model_mat;

	//Draw grid
	glUseProgram(gridProgram);
	glUniformMatrix4fv(uniforms.grid.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
	glDrawArrays(GL_POINTS, 0, 1);

	//Draw Point and curves
	glUseProgram(basicProgram);
	glUniformMatrix4fv(uniforms.basic.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
	glUniformMatrix4fv(uniforms.basic.model_matrix, 1, GL_FALSE, (GLfloat*)&model_mat[0][0]);

	setColorToFragment(chaikinCurveColor);
	majBuffer(vertexBufferPoints, chaikinCurve);
	glBindVertexArray(vaoPoint);
	glDrawArrays(GL_POINTS, 0, chaikinCurve.size());
	glBindVertexArray(0);

	glBindVertexArray(vaoPoint);
	glDrawArrays(GL_LINE_STRIP, 0, chaikinCurve.size());
	glBindVertexArray(0);

	setColorToFragment(originalCurveColor);
	majBuffer(vertexBufferPoints, originalCurve);
	glBindVertexArray(vaoPoint);
	glDrawArrays(GL_LINE_STRIP, 0, originalCurve.size());
	glBindVertexArray(0);

	glUseProgram(0);
}

void callbackMousePos(GLFWwindow *window, int button, int action, int mods)
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
			cam->releaseCam();
		else
			cam->grabCam(x, y);
	}
}

void callbackMouseMove(GLFWwindow *window, double x, double y)
{
	cam->orienterCam(x, y);
}

void callbackKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		switch (key)
		{
			case GLFW_KEY_Z:
				cam->deltaForward = -1;
				break;
			case GLFW_KEY_S:
				cam->deltaForward = 1;
				break;
			case GLFW_KEY_Q:
				cam->deltaStrafe = -1;
				break;
			case GLFW_KEY_D:
				cam->deltaStrafe = 1;
				break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case GLFW_KEY_Z:
			case GLFW_KEY_S:
				cam->deltaForward = 0;
				break;
			case GLFW_KEY_Q:
			case GLFW_KEY_D:
				cam->deltaStrafe = 0;
				break;
		}
	}
}

void initialize()
{
	/*VAO Points*/
	glGenVertexArrays(1, &vaoPoint);
	glBindVertexArray(vaoPoint);

	glGenBuffers(1, &vertexBufferPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * chaikinCurve.size(), chaikinCurve.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(uniforms.basic.position);
	glVertexAttribPointer(uniforms.basic.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

void loadShaders()
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

	gridProgram = gridShader.GetProgram();
	basicProgram = basicShader.GetProgram();

	uniforms.grid.projview_matrix = glGetUniformLocation(gridProgram, "projview_matrix");

	uniforms.basic.projview_matrix = glGetUniformLocation(basicProgram, "projview_matrix");
	uniforms.basic.model_matrix = glGetUniformLocation(basicProgram, "model_matrix");
	uniforms.basic.position = glGetAttribLocation(basicProgram, "a_position");
	uniforms.basic.color = glGetUniformLocation(basicProgram, "fragmentColor");
}

int main(int argc, char** argv)
{
	cam = new Camera();

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Coons & Subvision", NULL, NULL);
    glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	gl3wInit();
	
    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

	glfwSetMouseButtonCallback(window, callbackMousePos);
	glfwSetCursorPosCallback(window, callbackMouseMove);
	glfwSetKeyCallback(window, callbackKeyboard);

	loadShaders();
	initialize();

    bool show_test_window = false;

	originalCurve.push_back(glm::vec3(0, 0.25, 0));
	originalCurve.push_back(glm::vec3(1, 0.25, 0));
	originalCurve.push_back(glm::vec3(1, 1.25, 0));

	majBuffer(vertexBufferPoints, chaikinCurve);

	for (int i = 0; i < chaikinCurve.size(); i++)
		printf("Point %d: %f - %f - %f\n", i, chaikinCurve[i].x, chaikinCurve[i].y, chaikinCurve[i].z);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Chaikin Curve / Coons / Subdivision");
		ImGui::Text("Parameters for chaikin curve");

		ImGui::ColorEdit3("chaikin curve color", (float*)&chaikinCurveColor);
		ImGui::ColorEdit3("original curve color", (float*)&originalCurveColor);

		ImGui::SliderInt("Iteration for chaikin curve", &iterations, 1, 5);

		ImGui::Spacing();
		ImGui::Separator();

		ImGui::Text("Parameters for coons");

		ImGui::Spacing();
		ImGui::Separator();

        if (ImGui::Button("Test Window")) show_test_window ^= 1;
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

		cam->updatePos();
		chaikinCurve = GetChaikinCurve(originalCurve, iterations);

        // Rendering
        glViewport(0, 0, width, height);
		glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPointSize(10);

		render();
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}

void setColorToFragment(ImVec4 &color)
{
	fragmentColor[0] = color.x;
	fragmentColor[1] = color.y;
	fragmentColor[2] = color.z;
	fragmentColor[3] = color.w;
	glProgramUniform4fv(basicProgram, uniforms.basic.color, 1, fragmentColor);
}

template<typename T>
void majBuffer(int vertexBuffer, std::vector<T> &vecteur)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vecteur.size(), vecteur.data(), GL_STATIC_DRAW);
}

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
GLint gridProgram;
int width = 1500;
int height = 800;

struct
{
	struct
	{
		int     projview_matrix;
	} grid;
} uniforms;

using namespace glm;

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
	
	//Draw grid
	glUseProgram(gridProgram);
	glUniformMatrix4fv(uniforms.grid.projview_matrix, 1, GL_FALSE, (GLfloat*)&proj_view[0][0]);
	glDrawArrays(GL_POINTS, 0, 1);
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
	if (action == GLFW_PRESS)
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

void loadShaders()
{
	EsgiShader gridShader;

	printf("Load grid fs\n");
	gridShader.LoadFragmentShader("shaders/grid.frag");
	printf("Load grid vs\n");
	gridShader.LoadVertexShader("shaders/grid.vert");
	printf("Load grid geom\n");
	gridShader.LoadGeometryShader("shaders/grid.geom");
	gridShader.Create();

	gridProgram = gridShader.GetProgram();
	uniforms.grid.projview_matrix = glGetUniformLocation(gridProgram, "projview_matrix");
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

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);


	std::vector<glm::vec3> courbe;
	courbe.push_back(glm::vec3(0, 0, 0));
	courbe.push_back(glm::vec3(1, 0, 0));
	courbe.push_back(glm::vec3(1, 1, 0));

	std::vector<glm::vec3> chaikinCurve = GetChaikinCurve(courbe, 2);

	for (int i = 0; i < chaikinCurve.size(); i++)
		printf("Point %d: %f - %f - %f\n", i, chaikinCurve[i].x, chaikinCurve[i].y, chaikinCurve[i].z);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

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

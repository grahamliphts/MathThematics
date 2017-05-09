// maths.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include"imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GL\gl3w.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>


//#include <>

using namespace glm;
int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global)
	window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW

	gl3wInit();
	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do {
		// Draw nothing, see you in tutorial 2 !


			ImGui_ImplGlfwGL3_NewFrame();

			ImGui::Begin("Convex Hull & Voronoi");
			ImGui::Text("This window is here to use the application!");

			ImGui::Columns(2, "mixed");
			ImGui::Separator();
			//ImGui::Checkbox("Triangulation", &triangulationEnabled);
			//ImGui::Checkbox("Enhance Triangulation", &enhanceTriangulation);

			ImGui::NextColumn();
			//ImGui::Checkbox("Voronoi", &voronoiEnabled);
			//ImGui::Checkbox("Enhance Voronoi", &enhanceVoronoi);

			ImGui::Columns(1);
			ImGui::Separator();

			ImGui::Columns(2, "mixed");
			ImGui::Separator();

			ImGui::Text("Graphic Manipulation");
			ImGui::Render();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

    return 0;
}


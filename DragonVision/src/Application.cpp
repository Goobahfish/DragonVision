#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Texture.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"
#include "tests/BlockLoader.h"
#include "tests/Test.h"

int main(void)
{
	GLFWwindow* window;

	int window_width  = 800;
	int window_height = 600;

	// ############################
	// ## Initialise the library ##
	// ############################
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// ##########################################################
	// ## Create a windowed mode window and its OpenGL context ##
	// ##########################################################
	window = glfwCreateWindow(window_width, window_height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// #######################################
	// ## Make the window's context current ##
	// #######################################
	glfwMakeContextCurrent(window);

	// #################################################
	// ## Initialises GLEW                            ##
	// ## Must be done after 'glfwMakeContextCurrent' ##
	// #################################################
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
		return -1;
	}

	// #############################################
	// ## Some stuff

	// ## Enable Blending (i.e. alpha rules)
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// #############
	// ## Shaders ##
	// #############

	Shader shader("res/shaders/BasicShader.shader");
	shader.Bind();

	// #############
	// ## Texture ##
	// #############

	Texture texture("res/textures/Pink.png");

	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);

	// #######################################################################

	float r = 0.0f;
	float increment = 0.01f;

	// ## Clearing
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	*/
	shader.Unbind();

	Renderer renderer;

	// ## IMGUI Initialisation
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	test::Test * current_test = nullptr;
	test::TestMenu * test_menu = new test::TestMenu(current_test);
	current_test = test_menu;

	test_menu->RegisterTest<test::TestClearColor>("Color");
	test_menu->RegisterTest<test::TestTexture2D>("Texture 2D");
	test_menu->RegisterTest<test::TestTexture3D>("Texture 3D");
	test_menu->RegisterTest<test::BlockLoader>("Block Loader");
	/*
	glm::vec3 translationA(0.2f, 0.2f, 0);

	glm::vec3 translationB(0.2f, -0.2f, 0);
	*/
	// ###########################################
	// ## Loop until the user closes the window ##
	// ###########################################
	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		// ## The Important start of things
		// ## Render Here
		renderer.Clear();

		// ## IMGUI Running
		ImGui_ImplGlfwGL3_NewFrame();


		if (current_test)
		{
			current_test->OnUpdate(0.0f);
			current_test->OnRender();
			ImGui::Begin("Test");
			if (current_test != test_menu && ImGui::Button("<-"))
			{
				delete current_test;
				current_test = test_menu;
			}
			current_test->OnImGuiRender();
			ImGui::End();
		}

		//test.OnImGuiRender();
		
		/*
		// ## Shader
		shader.Bind();
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
		}
	
		
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
		}
		

		// ## Some fun
		if (r > 1.0f)
		{
			increment = -0.01f;
		}
		else if (r < 0.0f)
		{
			increment = 0.01f;
		}
		r += increment;

		// ## IMGUI Window
		{
			static float f = 0.0f;
			ImGui::SliderFloat3("Translation A", &translationA.x, -1.0f, 1.0f);
			ImGui::SliderFloat3("Translation B", &translationB.x, -1.0f, 1.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		*/
		
		// ## IMGUI Render
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// ## I think this is basically the draw (swapping the two buffers)
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete current_test;
	if (current_test != test_menu)
	{
		delete test_menu;
	}

	//glDeleteProgram(shader);

	// ## IMGUI Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	// Delete

	glfwTerminate();
	return 0;
}
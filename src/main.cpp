#define GLFW_INCLUDE_GLCOREARB
#define PI 3.14159265
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "../include/shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void rotateSqr(float *vertices, float radian, float radius);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
	//positions			//colors
	0.0f, 0.5f, 0.0f, 	1.0f, 0.0f, 0.0f, // top
	0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // bottom
	-0.5f, 0.0f, 0.0f, 	0.0f, 0.0f, 1.0f, // left
	0.5f, 0.0f, 0.0f, 	1.0f, 1.0f, 0.0f  // right
};
unsigned int indices[] = {
	0, 1, 3,
	0, 2, 1
};

int main(){
	//glfw initiate and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Draw Triangle", 
		NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//create shader program
	Shader shader("shader/vshader.vs", "shader/fshader.fs");

	//create VAO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind VAO and VBO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//vertex color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind VAO, VBO and EBO, optional
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	float radian = 0; //used for ratating the squre
	//render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//render the triangle
		shader.use();
		glBindVertexArray(VAO);

		//rebuffer changed data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		if (radian >= 2 * PI){
			rotateSqr(vertices, 0, 0.5);
			radian = 0;
		} else {
			radian += 0.01;
			rotateSqr(vertices, radian, 0.5);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//rotate the given squre by given radian
//if the squre has been rotated by 90 degrees, reset to its origin value
// vertices[0]: top
// vertices[6]: bottom
// vertices[12]: left
// vertices[18]: right
void rotateSqr(float *vertices, float radian, float radius){
	float x_top = sin(radian) * radius;
	float y_top = cos(radian) * radius;
	vertices[0] = x_top;
	vertices[1] = y_top;
	vertices[6] = -x_top;
	vertices[7] = -y_top;
	// vertices[3] = -x_top;
	// vertices[4] = -y_top;
	vertices[12] = -y_top;
	vertices[13] = x_top;
	// vertices[6] = -y_top;
	// vertices[7] = x_top;	
	vertices[18] = y_top;
	vertices[19] = -x_top;
	// vertices[9] = y_top;
	// vertices[10] = -x_top;
}

//this callback function is called whenever the window size is changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}



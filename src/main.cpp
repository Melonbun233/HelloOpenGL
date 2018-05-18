#define STB_IMAGE_IMPLEMENTATION
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/data.h"

using namespace std;
using namespace glm;

#define PI 3.14159265
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const char *v_shader_path = "../resources/shader/vshader.vs";
const char *f_shader_path = "../resources/shader/fshader.fs";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void rotateSqr(float *vertices, float radian, float radius);
void configTexture(const char *path, int texture);

//field of view
float FOV = 50.0;
//value used for mixing two textures
float mix_value = 0.2;
float rotate_value = 0;

extern float cube_vertices[];
extern vec3 cube_pos[];


int main(){
	//glfw initiate and configure
	//----------------initiate window and other stuffs-----------------//
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", 
		NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	
	//create shader program
	//the executable file is generated under /bin directory.
	//however, shader files are under ../resources/shader directory.
	Shader shader(v_shader_path, f_shader_path);

	//------------------------Vertices and Data-------------------------//
	//create VAO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, 
		GL_STATIC_DRAW);

	//vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture vertices
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind VAO, VBO and EBO, optional
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//---------------------------------Texture----------------------------//

	//generate texture
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	const char *path1 = "../resources/textures/container.jpg";
	const char *path2 = "../resources/textures/face.png";
	//configuring textures
	configTexture(path1, texture1);
	configTexture(path2, texture2);
	//set uniform in shader
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	//-------------------------transformation--------------------------------//

	//creating model matrix. used to transform local space to world space
	//this is set in the rendering loop
	// mat4 model;
	// model = rotate(model, radians(50.0f), vec3(0.5f, 1.0f, 0.0f));

	//creating view matrix, used to transform world space to user view
	mat4 view;
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));

	//creating proejction matrix, used for perspective projection
	mat4 proj;
	proj = perspective(radians(FOV), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

	//sent matrices to vertex shader
	shader.setMat4("view", view);
	shader.setMat4("proj", proj);
	mat4 rotation;

	//--------------------------setting up a camera-------------------------//
	vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f); //origin of the space
	vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	//-------------------------rendering------------------------------------//
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		//enable depth test for 3d objects
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//render
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//configure shader

		shader.setFloat("mix_value", mix_value);
		shader.use();

		glBindVertexArray(VAO);
		rotation = rotate(rotation, radians(1.0f), vec3(0.5f, 1.0f, 0.0f));
		//draw 10 cubes
		for (int i = 0; i < 10 ; i ++) {
			mat4 model;
			model = translate(model, cube_pos[i]);
			float angle = 20.0f * i;
			model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
			model = model * rotation;
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mix_value += mix_value >= 1.0f ? 0 : 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mix_value -= mix_value <= 0.0f ? 0 : 0.01f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotate_value += rotate_value >= 0.1 ? 0 : 0.01f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotate_value -= rotate_value <= -0.1 ? 0 : 0.01f;
}

//this callback function is called whenever the window size is changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

void configTexture(const char *path, int texture){
	int width, height, channels;
	glBindTexture(GL_TEXTURE_2D, texture);
	//set texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//loading picture 
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
	if (data) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		cout << path << " texture successfully loaded" << endl;
	}
	else 
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
}



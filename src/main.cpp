#define STB_IMAGE_IMPLEMENTATION
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/shader.h"
#include "../include/config.h"
#include "../include/data.h"

using namespace std;
using namespace glm;

#define PI 3.14159265
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float FOV_MAX = 45.0f;
const float FOV_MIN = 1.0f;
const char *v_shader_path = "../resources/shader/vshader.vs";
const char *f_shader_path = "../resources/shader/fshader.fs";

float delta_time = 0.0f; //time between current frame and last frame
float current_frame = 0.0f;	//current frame time
float last_frame = 0.0f;	//time of the last frame

//value used for mixing two textures
float mix_value = 0.2;

//--------------------------setting up mouse---------------------------//

//mouse initial position, center of the screen
int MOUSE_VERTICAL_INVERSE = 1;
int MOUSE_HORIZONTAL_INVERSE = 0;
int MOUSE_FIRST = 1; //mouse first time enter the screen
//mouse sensitivity
float MOUSE_VERTICAL_SENS = 0.05f;
float MOUSE_HORIZONTAL_SENS = 0.05f;
//mouse initial position
float MOUSE_X = SCR_WIDTH / 2;
float MOUSE_Y = SCR_HEIGHT / 2;
//vertices data
extern float cube_vertices[];
extern vec3 cube_pos[];

//--------------------------setting up a camera-------------------------//
//field of view
float FOV = 45.0;
vec3 cameraPos = vec3(0.0f, 0.0f, 5.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
//yaw: horizontal movement; pitch: vertical movement
float yaw, pitch; //camera movement by mouse
//walk speed when press wsad
float camera_speed = 5.0f * delta_time;



int main(){
	//----------------initiate window and other stuffs-----------------//
	//glfw initiate and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//MOUSE_VERTICAL_SENS = 0.01f;
	//MOUSE_HORIZONTAL_SENS = 0.15f;
#endif

#ifdef __WINDOWS__
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	//creating proejction matrix, used for perspective projection
	mat4 proj;

	//sent matrices to vertex shader
	shader.setMat4("proj", proj);
	mat4 rotation;

	//-------------------------rendering------------------------------------//
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		//update frame timer
		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

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
		//configure model, view, projection
		//camera rotation
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		proj = perspective(radians(FOV), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("proj", proj);
		//cubes rotation
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








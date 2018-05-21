#include "../include/config.h"
//this file contains all config functions 

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

//process user input
void processInput(GLFWwindow *window){
	//set input mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mix_value += mix_value >= 1.0f ? 0 : 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mix_value -= mix_value <= 0.0f ? 0 : 0.01f;

	//walking
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeypad(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeypad(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeypad(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeypad(RIGHT, delta_time);
}

//this callback function is called whenever the window size is changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

//call back function whenever mouse is moved
void mouse_callback(GLFWwindow *window, double x, double y){
	//calculate difference mouse position from the last frame
	if (MOUSE_FIRST){
		MOUSE_FIRST = 0;
		MOUSE_X = x;
		MOUSE_Y = y;
	}
	float x_offset = x - MOUSE_X;
	float y_offset = y - MOUSE_Y;
	//update current position
	MOUSE_X = x;
	MOUSE_Y = y;
	camera.processMouseMovement(x_offset, y_offset);
}

//call back function whenever mouse is scolled
//y is the value that mouse scrolled
void scroll_callback(GLFWwindow *window, double x, double y){
	camera.processMouseScroll(y);
}
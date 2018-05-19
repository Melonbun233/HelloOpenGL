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

	camera_speed = 2.5f * delta_time;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mix_value += mix_value >= 1.0f ? 0 : 0.01f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mix_value -= mix_value <= 0.0f ? 0 : 0.01f;

	//walking
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += camera_speed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= camera_speed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;

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
	//std::cout << x_offset << " " << y_offset << std::endl;
	x_offset *= MOUSE_HORIZONTAL_SENS;
	y_offset *= MOUSE_VERTICAL_SENS;
	//update camera direction
	yaw += MOUSE_HORIZONTAL_INVERSE ? -x_offset : x_offset;
	pitch += MOUSE_VERTICAL_INVERSE ? -y_offset : y_offset;
	//std::cout << yaw << " " << pitch << std::endl;
	//restraints
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = 89.0f;
	//uodate camera direction
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	//std::cout << front.x << " " << front.y << " " << front.z << std::endl;
	cameraFront = glm::normalize(front);

}

//call back function whenever mouse is scolled
//y is the value that mouse scrolled
void scroll_callback(GLFWwindow *window, double x, double y){
	if (FOV >= FOV_MIN && FOV <= FOV_MAX)
		FOV -= y;
	if (FOV <= FOV_MIN)
		FOV = FOV_MIN;
	if (FOV >= FOV_MAX)
		FOV = FOV_MAX;
}
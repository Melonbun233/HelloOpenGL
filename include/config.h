#ifndef UTILS_H
#define UTILS_H
//this file contains all config functions used in this project, including the extern
//variables that are used in main function.
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include "camera.h"

using namespace std;
//load texture and configure it as GL_REPEAT and GL_LINEAR for magnification and 
//minization
//PRE:
//	path: path of the texture file, should be a image file
//	texture: texture int created by OpenGL function
void configTexture(const char *path, int texture);

//process user input in the render loop
//PRE:
// window: user's window
extern Camera camera;
extern float delta_time;
extern float mix_value; // variable declared in main.cpp, used to mix two textures
void processInput(GLFWwindow *window);

//this function is automatically called every time the window is resized
//PRE:
//	window: user's window
//	width: new window's width
//  height: new window's height
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//this function is automatically called every time the window is resized
extern GLboolean MOUSE_FIRST;
extern float MOUSE_X, MOUSE_Y; //mouse's position
void mouse_callback(GLFWwindow *window, double x, double y);

//this function is automatically called every time the mouse is scrolled
void scroll_callback(GLFWwindow *window, double x, double y);

#endif 
#ifndef UTILS_H
#define UTILS_H
//this file contains all utility functions used in this project
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

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
extern float mix_value; // variable declared in main.cpp, used to mix two textures
void processInput(GLFWwindow *window);

//this function is automatically called every time the window is resized
//PRE:
//	window: user's window
//	width: new window's width
//  height: new window's height
void framebuffer_size_callback(GLFWwindow *window, int width, int height);





#endif 
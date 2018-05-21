// this is the header file of a camera class
// this camera is able to detect user's certain input. eg. wsad and mouse movement
#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
using namespace std;

//4 different directions used to control the camera
enum MOVEMENT {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float FOV_MAX = 45.0;
const float FOV_MIN = 0.1;
const float INIT_YAW = -90.0;
const float INIT_PITCH = 0.0;
const float INIT_SPEED = 2.5;
const float INIT_MOUSE_SENSITIVITY = 0.05;

class Camera {
public:
	GLboolean MOUSE_VERTICAL_INVERSE;
	GLboolean MOUSE_HORIZONTAL_INVERSE;
	glm::vec3 cameraPos, cameraFront, cameraUp; //used to generate lookat matrix

	//construct camera with vectors
	//the camera obeject requires a initial position vector, a front vector indicating 
	//camera's direction, and a vector indicates world's up vector, usually (0, 1, 0)
	//user can also indicate camera's initial yaw and pitch
	//if no parameter is provieded, the camera will generated at world's (0, 0, 0) position,
	//pointing to negative z-axis
	//mouse move inversing is set to false by default
	// input arguments: (position, front, up, yaw, pitch)
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float yaw = INIT_YAW, float pitch = INIT_PITCH);

	//this function processes the camera movement of the camera
	//PRE: 
	//	direction: one of the four directions
	//  delta_time: time difference between last frame and current frame,
	//		this is used to calculate movement speed, so that camera speed is constant
	//		no matter how many fps you have
	void processKeypad(MOVEMENT direction, float delta_time);

	//this function proceses the mouse movement, witch will affects the view that camera
	//is pointing to.
	//PRE:
	//	x_offset: offset that mouse moves horizontally
	// 	y_offset: offset that mouse moves vertically
	//	constrainPitch: whether constrain pitch between -90 and 90 degrees. This should be 
	//		set to true in most cases. Otherwise, there might be strange camera views.
	void processMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = true);

	//this function process mouse scrolling. The fov of the camera will be affected while 
	//scrolling the mouse.
	//PRE:
	// 	positive scroll_value will results in zooming in, and negative scroll_value will 
	//	results in zooming out.
	void processMouseScroll(double scroll_value);

	//this function should be called in the rendering loop and sent the returning matrix
	//to the vertex shader
	//POST:
	//	a matrix will be returned as view matrix (presenting current camera direction and position)
	glm::mat4 getView();
	//fov getter
	float getFOV();
	//speed setter, note original camera speed is 2.5f
	void setSpeed(float speed);
	//mouse sensitivity setter
	void setMouseSensitivity(float sens);
	//horizontal inverse setter
	void setMouseHorizontalInverse(GLboolean inverse);
	//vertical inverse setter
	void setMouseVerticalInverse(GLboolean inverse);

private:
	float _yaw, _pitch; //yaw and pitch should be updated every time user moves it's mouse
	float _fov;	//current camera fov (field of view)
	float _speed;
	float _mouse_sens;

	//update camera's vectors, this function is called everytime the mouse is moved
	void updateCamera();

};

#endif
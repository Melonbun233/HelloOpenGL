#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;

class Shader{

public: 
	//constructor that builds and reads the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	//shader program ID
	int ID;

	//this function should be called before each rendering
	void use();

	//set a bool uniform in the shader
	void setBool(const string &name, bool value) const;

	//set a integer uniform in the shader
	void setInt(const string &name, int value) const;

	//set a float uniform in the shader
	void setFloat(const string &name, float value) const;

	//set a mat4 unifrom in the shader
	void setMat4(const string &name, mat4 value) const;


private:
	// check whether shader is compiled succesfully
	void checkShaderSuccess(unsigned int shader);
	// check whether shader program is succesfully linked
	void checkLinkSuccess(unsigned int ID);

};


#endif
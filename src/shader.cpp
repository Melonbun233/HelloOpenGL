// this is the shader source code for the shader class
#include "../include/shader.h"

//use this shader program
void Shader::use(){
	glUseProgram(ID);
}

//set a integer uniform in the shader
void Shader::setInt(const string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

//set a bool uniform in this shader
void Shader::setBool(const string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

//set a float uniform in the shader
void Shader::setFloat(const string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

//set a mat4 unifrom in the shader
void Shader::setMat4(const string &name, mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
	 value_ptr(value));
}

//constructor
Shader::Shader(const char* vertexPath, const char* fragmentPath){
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		//check ifstream objects can throw exception
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			//read file's buffer contentes into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			//cout << vertexCode << endl;
			fragmentCode = fShaderStream.str();
			//cout << fragmentCode << endl;
		} catch (ifstream::failure e) {
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//compile shader
		int vertex, fragment;

		//vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkShaderSuccess(vertex);
		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkShaderSuccess(fragment);

		//shader programs
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkLinkSuccess(ID);

		//delete the shaders as they're linked into the shader program
		glDeleteShader(vertex);
		glDeleteShader(fragment);
}

// check whether shader is compiled succesfully
void Shader::checkShaderSuccess(unsigned int shader){
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "Shader compilation error\n" << infoLog << endl;
		return;
	}
	cout << "Shader compilation successful" << endl;
}
// check whether shader program is succesfully linked
void Shader::checkLinkSuccess(unsigned int ID){
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "Shader Program Linking Error\n" << infoLog << endl;
		return;
	}

	cout << "Shader program linking successful" << endl;
}
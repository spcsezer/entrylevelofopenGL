#pragma once

#include <fstream>
#include <string>
#include <iostream>
using namespace std;
 
#include <glew.h>

#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>
using namespace glm;

class Shader {

private:
	//Member Variables
	GLuint id;

	string loadShaderSource(char* fileName) {

		string temp = "";
		string src = "";

		ifstream in_file;

		//vertex
		in_file.open(fileName);

		if (in_file.is_open()) {

			while (getline(in_file, temp))
				src += temp + "\n";

		}
		else {
			cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: "<< fileName << endl;
		}

		in_file.close();

		return src;
	}

	GLuint loadShader(GLenum type, char* fileName) {

		char infoLog[512];
		GLint success;

		GLuint shader = glCreateShader(type); //create vertex shader in the background
		//we give source 
		string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		//check compile error
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {

			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: "<< fileName << endl;
			cout << infoLog << endl;

		}

		return shader;

	}

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {

		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();
		glUseProgram(this->id);

		glAttachShader(this->id, vertexShader);

		if (geometryShader) {

			glAttachShader(this->id, geometryShader);

		}
		
		glAttachShader(this->id, fragmentShader);
		
		glLinkProgram(this->id);

		//check error
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success) {

			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << endl;
			cout << infoLog << endl;

		}

		glUseProgram(0);

	}


public:

	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = "") {
		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, (char*)vertexFile);

		if (geometryFile != "") {
			geometryShader = loadShader(GL_GEOMETRY_SHADER, (char*)geometryFile);

		}

		fragmentShader = loadShader(GL_FRAGMENT_SHADER, (char*)fragmentFile);

		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		//end
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);

	}

	~Shader() {

		glDeleteProgram(this->id);

	}

	//Set Uniform Functions

	void use() {

		glUseProgram(this->id);

	}

	void unuse(){

		glUseProgram(0);

	}

	void set1i(GLint value, const GLchar* name) {

		this->use();

		glUniform1i(glGetUniformLocation(this->id, name), value);

		this->unuse();

	}

	void setVec1f(GLfloat value, const GLchar* name) {

		this->use();

		glUniform1f(glGetUniformLocation(this->id, name), value);

		this->unuse();

	}

	void setVec2f(fvec2 value, const GLchar* name) {

		this->use();

		glUniform2fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));

		this->unuse();

	}

	void setVec3f(fvec3 value, const GLchar* name) {

		this->use();

		glUniform3fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));

		this->unuse();

	}

	void setVec4f(fvec4 value, const GLchar* name) {

		this->use();

		glUniform4fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));

		this->unuse();

	}

	void setMat3fv(mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE) {

		this->use();

		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));

		this->unuse();

	}

	void setMat4fv(mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE) {

		this->use();

		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));

		this->unuse();

	}

};

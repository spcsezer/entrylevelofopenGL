#pragma once

#include <string>
#include <iostream>
using namespace std;

#include <glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2.h>

class Texture {

private:

	GLuint id;
	int width, height;
	unsigned int type;
	GLint textureUnit;

public:

	Texture(const char* fileName, GLenum type, GLint texture_unit) {

		this->type = type;
		this->textureUnit = texture_unit;

		unsigned char* texturedata = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id); //one image
		glBindTexture(type, this->id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);  //S and T other names is x and y
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);  //if image doesn't fit the window then repeat until fit
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);  //antialising
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (texturedata) {

			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
			glGenerateMipmap(type); //mipmap generates same image in several different sizes images

		}
		else {
			cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: "<< fileName << endl;
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(texturedata);

	}

	~Texture() {

		glDeleteTextures(1, &this->id);

	}

	inline GLuint getID() const {

		return this->id;

	}

	void bind() {

		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->id);

	}

	void unbind() {

		glActiveTexture(0);
		glBindTexture(this->type, 0);

	}

	inline GLint getTextureUnit() const{

		return this->textureUnit;

	}

	void loadFromFile(const char* fileName) {

		if (this->id) {

			glDeleteTextures(1, &this->id);

		}

		unsigned char* texturedata = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id); //one image
		glBindTexture(this->type, this->id);

		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);  //S and T other names is x and y
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);  //if image doesn't fit the window then repeat until fit
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);  //antialising
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (texturedata) {

			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
			glGenerateMipmap(this->type); //mipmap generates same image in several different sizes images

		}
		else {
			cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << endl;
		}

		glActiveTexture(0);
		glBindTexture(this->type, 0);
		SOIL_free_image_data(texturedata);

	}

};
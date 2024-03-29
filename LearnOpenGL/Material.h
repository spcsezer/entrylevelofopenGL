#pragma once

#include<glew.h>
#include<GLFW/glfw3.h>

#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc\type_ptr.hpp>

#include"Shader.h"

using namespace glm;

class Material
{
private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

public:
	Material(
		vec3 ambient,
		vec3 diffuse,
		vec3 specular,
		GLint diffuseTex,
		GLint specularTex
	)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material() {}

	//Function
	void sendToShader(Shader& program)
	{
		program.setVec3f(this->ambient, "material.ambient");
		program.setVec3f(this->diffuse, "material.diffuse");
		program.setVec3f(this->specular, "material.specular");
		program.set1i(this->diffuseTex, "material.diffuseTex");
		program.set1i(this->specularTex, "material.specularTex");
	}
};
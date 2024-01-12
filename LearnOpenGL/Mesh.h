#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Primitives.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class Mesh {

private:

	unsigned nrOfVertices;
	unsigned nrOfIndicies;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	mat4 ModelMatrix;

	void initVAO(Primitive* primitive) {

		this->nrOfVertices = primitive->getNrOfVertices();
		this->nrOfIndicies = primitive->getNrOfIndices();

		//vertexbufferobject, elementbufferobject
		//VAO, VBO, EBO          These are buffers and they are send our data to graphic card memory
		glCreateVertexArrays(1, &this->VAO); // we created a shell to use
		glBindVertexArray(this->VAO); //This VAO active now

		//generate VBO and bind and send data
		glGenBuffers(1, &this->VBO);  //create space
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW); //this is the data sending to the graphic card

		//generate EBO and bind and send data
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndicies * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

		//set vertexattribpointers and enable
		//order position, color and texcoords for graphic card can understand these
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)); // this is gonna represent our position
		glEnableVertexAttribArray(0);
		//color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color)); // this is gonna represent our position
		glEnableVertexAttribArray(1);
		//texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord)); // this is gonna represent our position
		glEnableVertexAttribArray(2);
		//normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)); // this is gonna represent our position
		glEnableVertexAttribArray(3);

		//bind VAO 0
		glBindVertexArray(0);

	}

	void initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndicies) {

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndicies = nrOfIndicies;

		//vertexbufferobject, elementbufferobject
	    //VAO, VBO, EBO          These are buffers and they are send our data to graphic card memory
		glCreateVertexArrays(1, &this->VAO); // we created a shell to use
		glBindVertexArray(this->VAO); //This VAO active now

		//generate VBO and bind and send data
		glGenBuffers(1, &this->VBO);  //create space
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW); //this is the data sending to the graphic card

		//generate EBO and bind and send data
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndicies * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

		//set vertexattribpointers and enable
		//order position, color and texcoords for graphic card can understand these
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)); // this is gonna represent our position
		glEnableVertexAttribArray(0);
		//color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color)); // this is gonna represent our position
		glEnableVertexAttribArray(1);
		//texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord)); // this is gonna represent our position
		glEnableVertexAttribArray(2);
		//normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)); // this is gonna represent our position
		glEnableVertexAttribArray(3);

		//bind VAO 0
		glBindVertexArray(0);

	}

	void updateUniforms(Shader* shader) {

		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");

	}

	void updateModelMatrix() {

		this->ModelMatrix = mat4(1.f); //4*4 matrix
		this->ModelMatrix = translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);

	}

public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndicies,
		vec3 position = vec3(0.f), vec3 rotation = vec3(0.f), vec3 scale = vec3(1.f)) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndicies);
		this->updateModelMatrix();

	}

	Mesh(Primitive* primitive,
		vec3 position = vec3(0.f), vec3 rotation = vec3(0.f), vec3 scale = vec3(1.f)) {

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->initVAO(primitive);
		this->updateModelMatrix();

	}

	~Mesh() {

		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);

	}

	//Accessors

	//Modifiers
	void setPosition(const vec3 position) {

		this->position = position;

	}

	void setRotation(const vec3 rotation) {

		this->rotation = rotation;

	}

	void setScale(const vec3 scale) {

		this->scale = scale;

	}


	//functions

	void move(const vec3 position) {

		this->position += position;

	}

	void rotate(const vec3 rotation) {

		this->rotation += rotation;

	}

	void scaleUp(const vec3 scale) {

		this->scale += scale;

	}

	void update() {



	}

	void render(Shader* shader) {

		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->use();

		//Bind vertex array object
		glBindVertexArray(this->VAO);

		//draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, this->nrOfIndicies, GL_UNSIGNED_INT, 0);

	}

};
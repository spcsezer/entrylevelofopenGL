#include "libs.h"

void updateInput(GLFWwindow* window, Mesh &mesh) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		mesh.move(vec3(0.f, 0.f, 0.01f));

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mesh.move(vec3(0.f, 0.f, -0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mesh.move(vec3(0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mesh.move(vec3(-0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		mesh.rotate(vec3(0.f, -1.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		mesh.rotate(vec3(0.f, 1.f, 0.f));
	}


	//if we gonna press escape key. Program will close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		glfwSetWindowShouldClose(window, GLFW_TRUE);

	}

}

int main()
{

	//init glfw
	glfwInit();

	//for modern opengl and new functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//version of opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); //means 4.4

	//cant resize
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//for mac users
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

	//window sizes
	const int window_width = 1024, window_height = 768;
	int framebufferWidth = 0, framebufferHeight = 0;
	//create window
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//canvas size and location
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	//init glew. For link functions from driver
	//for enable modern opengl functionality
	glewExperimental = GL_TRUE;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//check initialization ok
	if (glewInit() != GLEW_OK) {

		cout << "glew init failed" << endl;
		glfwTerminate();
	}

	//opengl options
	glEnable(GL_DEPTH_TEST); // how far a pixel is

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //we dont want to draw back face of model
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //We can set how look like the draw

	//shader init
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");
	/*GLuint core_program;
	if (!loadShaders(core_program))
		glfwTerminate();*/
	
	//Model mesh
	 
	//Mesh test(vertices, nrOfVertices, indices, nrOfIndices, vec3(0.f), vec3(0.f), vec3(2.f));
	Quad tempQuad = Quad();

	Mesh test(&tempQuad, vec3(0.f), vec3(0.f), vec3(2.f));
	
	//textures

	Texture texture0("images/container.png", GL_TEXTURE_2D, 0);
	Texture texture1("images/container1.png", GL_TEXTURE_2D, 1);

	//materials
	Material material0(vec3(1.f), vec3(1.0f), vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

	//matrix
	
	vec3 camPosition(0.f, 0.f, 2.f);
	vec3 worldUp(0.f, 1.f, 0.f);
	vec3 camFront(0.f, 0.f, -1.f);
	mat4 ViewMatrix(1.f);

	ViewMatrix = lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f, nearPlane = 0.1f, farPlane = 1000.f;
	mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = perspective(radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

	//Lights
	vec3 lightPos0(0.f, 0.f, 1.0f);


	//Init uniforms
	//core_program.use();
    //We sent the matrix to shader
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	//We sent point light position etc.
	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");

	//main loop
	while (!glfwWindowShouldClose(window)) {

		//update input
		glfwPollEvents(); //check cursor
		//update
		updateInput(window, test);

		//draw
		//clear  (clear the old frame and you can print the new frame)
		glClearColor(0.f, 0.f, 0.f, 1.f); //last one transparency. we set full opacity
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//update uniform. You can send shader cpu to gpu and for multiple textures
		core_program.set1i(texture0.getTextureUnit(), "texture0");
		core_program.set1i(texture1.getTextureUnit(), "texture1");
		//core_program.set1i(texture1.getTextureUnit(), "texture1");
		material0.sendToShader(core_program);

		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		//use a program
		core_program.use();

		//activate texture
		texture0.bind();
		texture1.bind();

		//draw

		test.render(&core_program);

		//end draw
		glfwSwapBuffers(window); //old frames and new frames swapping
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//end of program
	glfwDestroyWindow(window);
	glfwTerminate();

	//delete program

	return 0;
}
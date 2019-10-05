#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void do_movement();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
	void setupTexture(); //apenas mostra como criar uma textura
private:
	
	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model; 
	glm::mat4 model2;
	glm::mat4 model3;
	glm::mat4 model4;
	glm::mat4 model5;
	glm::mat4 model6;
	glm::mat4 model7;
	glm::mat4 model8;
	glm::mat4 modelC;
	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture, texture1, texture2,texture3,texture4 ,alvo, textureE2, textureE3, textureE4, textureE5, background, textureE7,textureG;

};


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

	void processInput();
	void render();
	void run();
	void finish();
	void calculateCollisions();

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
	glm::mat4 model_alvo;
	glm::mat4 model_flecha;
	glm::mat4 model_nuvens;
	glm::mat4 model_atirador;
	glm::mat4 model_background;
	//2D Camera - Projection matrix
	glm::mat4 projection;
	//Texture index
	unsigned int textura_flecha[5], textura_alvo[11], textura_cenario, textura_atirador[6], textura_nuvens;
	int spriteRefreshTick = 0, spriteAtualAlvo = 0, spriteAtualAtirador = 0, spriteAtualFlecha = 0;
};


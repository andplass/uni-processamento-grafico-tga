#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
//window attributes
static bool resized;
static GLuint width, height;
//atributos globais customizados
int pontuacao = 0;
//atributos do viewport
const float MAX_SCENE_HEIGHT = 10;
const float MIN_SCENE_HEIGHT = -10;
//atributos do alvo
const float POS_ALVO_HORIZONTAL = -11;
const float VELOCIDADE_MOV_ALVO = 0.0075;
const glm::vec3 escala_alvo = glm::vec3(2.0f, 2.0f, 0.0f);
float pos_alvo_vertical = 0;
//atributos do atirador
const float POS_ATIRADOR_HORIZONTAL = 11;
const float VELOCIDADE_MOV_ATIRADOR = 0.0085;
const glm::vec3 escala_atirador = glm::vec3(5.0f, 5.0f, 0.0f);
float pos_atirador_vertical = 0;
//atributos das flecha
const float VELOCIDADE_MOV_FLECHA = 0.045;
const float POS_FLECHA_HORIZONTAL_INICIAL = POS_ATIRADOR_HORIZONTAL;
const glm::vec3 escala_flecha = glm::vec3(2.9f, 0.6f, 0.0f);
float pos_flecha_horizontal = 0;
float pos_flecha_vertical = 0;
bool is_flecha_disparda = false;
//atributos do cenario
const glm::vec3 escala_cenario = glm::vec3(30.0f, 30.0f, 0.0f);
float pos_cenario_vertical = 0;
//outros atributos
const float ZERO = 0;
const float POS_NUVEM_Z = 0;
const float POS_PLAYER_Z = 0;
const float OUT_OF_BOUNDS = 50;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "TGA", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::processInput()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_W]) {
		if (pos_alvo_vertical < MAX_SCENE_HEIGHT-1) {
			pos_alvo_vertical += VELOCIDADE_MOV_ALVO;
		}
	}
	if (keys[GLFW_KEY_S]) {
		if (pos_alvo_vertical > MIN_SCENE_HEIGHT+1) {
			pos_alvo_vertical -= VELOCIDADE_MOV_ALVO;
		}
	}
	if (keys[GLFW_KEY_O]) {
		if (pos_atirador_vertical < MAX_SCENE_HEIGHT-1) {
			pos_atirador_vertical += VELOCIDADE_MOV_ATIRADOR;
		}
	}
	if (keys[GLFW_KEY_L]) {
		if (pos_atirador_vertical > MIN_SCENE_HEIGHT+1) {
			pos_atirador_vertical -= VELOCIDADE_MOV_ATIRADOR;
		}
	}
	if (keys[GLFW_KEY_SPACE]) {
		if (!is_flecha_disparda) {
			is_flecha_disparda = true;
			pos_flecha_vertical = pos_atirador_vertical;
			pos_flecha_horizontal = POS_FLECHA_HORIZONTAL_INICIAL;
			//posicionar a flecha na frente do atirador (y = altura do atirador atual se mantem) (x inicial = fixo)
		}
	}
}

void SceneManager::calculateCollisions()
{
	//FLECHA E ALVO
	if (pos_flecha_vertical <= pos_alvo_vertical + 1
		&& pos_flecha_vertical >= pos_alvo_vertical - 1
		&& pos_flecha_horizontal <= POS_ALVO_HORIZONTAL + 2) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
		is_flecha_disparda = false;
	}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	// Create transformations 

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	// Pass them to the shaders
	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	// Bind Textures using texture units
	glUniform1i(glGetUniformLocation(shader->Program, "texture"), 0);

	//CENARIO
	pos_cenario_vertical = -(pos_alvo_vertical + pos_atirador_vertical) / 6;
	glBindTexture(GL_TEXTURE_2D, textura_cenario);
	model_background = glm::mat4();
	model_background = glm::translate(model_background, glm::vec3(ZERO, pos_cenario_vertical, ZERO));
	model_background = glm::scale(model_background, escala_cenario);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_background));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//ALVO
	glBindTexture(GL_TEXTURE_2D, textura_alvo);
	model_alvo = glm::mat4();
	model_alvo = glm::translate(model_alvo, glm::vec3(POS_ALVO_HORIZONTAL, pos_alvo_vertical, POS_PLAYER_Z));
	model_alvo = glm::scale(model_alvo, escala_alvo);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_alvo));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//ATIRADOR
	glBindTexture(GL_TEXTURE_2D, textura_atirador);
	model_atirador = glm::mat4();
	model_atirador = glm::translate(model_atirador, glm::vec3(POS_ATIRADOR_HORIZONTAL, pos_atirador_vertical, POS_PLAYER_Z));
	model_atirador = glm::scale(model_atirador, escala_atirador);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_atirador));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//FLECHA
	if (pos_flecha_horizontal <= -18) {
		is_flecha_disparda = false;
		pos_flecha_horizontal = OUT_OF_BOUNDS;
	}
	if (is_flecha_disparda) {
		pos_flecha_horizontal -= VELOCIDADE_MOV_FLECHA;
		glBindTexture(GL_TEXTURE_2D, textura_flecha);
		model_flecha = glm::mat4();
		model_flecha = glm::translate(model_flecha, glm::vec3(pos_flecha_horizontal, pos_flecha_vertical, POS_PLAYER_Z));
		model_flecha = glm::scale(model_flecha, escala_flecha);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_flecha));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (!is_flecha_disparda || pos_flecha_horizontal <= -18) {
		is_flecha_disparda = false;
		pos_flecha_horizontal = OUT_OF_BOUNDS;
		glBindTexture(GL_TEXTURE_2D, textura_flecha);
		model_flecha = glm::mat4();
		model_flecha = glm::translate(model_flecha, glm::vec3(pos_flecha_horizontal, pos_flecha_vertical, POS_PLAYER_Z));
		model_flecha = glm::scale(model_flecha, escala_flecha);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_flecha));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	calculateCollisions();
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		processInput();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void SceneManager::setupScene()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	setupTexture();
}

void SceneManager::setupCamera2D()
{
	//corrigindo o aspecto
	float ratio;
	float xMin = -10.0, xMax = 10.0, yMin = -10.0, yMax = 10.0, zNear = -1.0, zFar = 1.0;
	if (width >= height)
	{
		ratio = width / (float)height;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void SceneManager::setupTexture()
{
	int width, height, nrChannels;
	//------------------ // CENARIO // ---------------------------------------------------------//
	glGenTextures(1, &textura_cenario);
	glBindTexture(GL_TEXTURE_2D, textura_cenario); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *textura_cenario_file = stbi_load("../textures/ceu_2.png", &width, &height, &nrChannels, 0);
	if (textura_cenario_file){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura_cenario_file);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(textura_cenario_file);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // FLECHA // ---------------------------------------------------------//
	glGenTextures(1, &textura_flecha);
	glBindTexture(GL_TEXTURE_2D, textura_flecha);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *textura_flecha_file = stbi_load("../textures/flecha.png", &width, &height, &nrChannels, 0);
	if (textura_flecha_file){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textura_flecha_file);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(textura_flecha_file);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // ALVO // ---------------------------------------------------------//
	glGenTextures(1, &textura_alvo);
	glBindTexture(GL_TEXTURE_2D, textura_alvo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *textura_alvo_file = stbi_load("../textures/alvo.png", &width, &height, &nrChannels, 0);
	if (textura_alvo_file) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textura_alvo_file);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(textura_alvo_file);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // ALVO // ---------------------------------------------------------//
	glGenTextures(1, &textura_atirador);
	glBindTexture(GL_TEXTURE_2D, textura_atirador);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* textura_atirador_file = stbi_load("../textures/anjo.png", &width, &height, &nrChannels, 0);
	if (textura_atirador_file) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textura_atirador_file);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(textura_atirador_file);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

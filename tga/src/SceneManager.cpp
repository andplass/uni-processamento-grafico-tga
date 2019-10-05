#include "SceneManager.h"
//static controllers for mouse and keyboard
static bool keys[1024];
//window attributes
static bool resized;
static GLuint width, height;
//custom global variables
int pontuacao = 0;
//viewport
const float MAX_SCENE_HEIGHT = 9;
const float MIN_SCENE_HEIGHT = -9;
//target location attributes
const float POS_ALVO_HORIZONTAL = -11;
float pos_alvo_vertical = -0;
//projectile location attributes
float pos_flecha_1_horizontal = -20;
float pos_flecha_2_horizontal = 18;
//background location attributes
const float POS_BACKGROUND = 0;

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


void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_W]) {
		if (pos_alvo_vertical < MAX_SCENE_HEIGHT) {
			for (int i = 0; i < 4; i++) {
				pos_alvo_vertical = pos_alvo_vertical + 0.005;
			}
		}
	}
	if (keys[GLFW_KEY_S]) {
		if (pos_alvo_vertical > MIN_SCENE_HEIGHT) {
			for (int i = 0; i < 4; i++) {
				pos_alvo_vertical = pos_alvo_vertical - 0.005;
			}
		}
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
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	//BACKGROUND
	glBindTexture(GL_TEXTURE_2D, background);
	modelC = glm::mat4();
	modelC = glm::translate(modelC, glm::vec3(POS_BACKGROUND, -3.5f, 0.0f));
	modelC = glm::scale(modelC, glm::vec3(35.0f, 25.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelC));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//ALVO
	glBindTexture(GL_TEXTURE_2D, alvo);
	model3 = glm::mat4();
	model3 = glm::translate(model3, glm::vec3(POS_ALVO_HORIZONTAL, pos_alvo_vertical, 0.0f));
	model3 = glm::scale(model3, glm::vec3(5.0f, 5.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//FLECHAS
	pos_flecha_1_horizontal = pos_flecha_1_horizontal + 0.005;
	glBindTexture(GL_TEXTURE_2D, texture3);
	model5 = glm::mat4();
	model5 = glm::translate(model5, glm::vec3(pos_flecha_1_horizontal, -7.0f, 0.0f));
	model5 = glm::scale(model5, glm::vec3(5.0f, 5.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (pos_flecha_1_horizontal >= 18)
		pos_flecha_1_horizontal = -13;

	pos_flecha_2_horizontal = pos_flecha_2_horizontal - 0.004;
	glBindTexture(GL_TEXTURE_2D, texture1);
	model2 = glm::mat4();
	model2 = glm::translate(model2, glm::vec3(pos_flecha_2_horizontal, -3.0f, 0.0f));
	model2 = glm::scale(model2, glm::vec3(5.0f, 5.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glBindTexture(GL_TEXTURE_2D, texture1);
	model2 = glm::mat4();
	model2 = glm::translate(model2, glm::vec3(pos_flecha_2_horizontal, -10.0f, 0.0f));
	model2 = glm::scale(model2, glm::vec3(5.0f, 5.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//COLISAO
	// FLECHA_B
	if (POS_ALVO_HORIZONTAL + 0.5 >= -0.5 - 2.5 - 3 
		&& POS_ALVO_HORIZONTAL + 0.5 <= 0.5 + 2.5 - 3 
		&& pos_alvo_vertical + 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical + 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL + 0.5 >= -0.5 - 2.5 - 3 
		&& POS_ALVO_HORIZONTAL + 0.5 <= 0.5 + 2.5 - 3 
		&& pos_alvo_vertical - 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical - 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL - 0.5 >= -0.5 - 2.5 - 3 
		&& POS_ALVO_HORIZONTAL - 0.5 <= 0.5 + 2.5 - 3 
		&& pos_alvo_vertical + 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical + 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL - 0.5 >= -0.5 - 2.5 - 3 
		&& POS_ALVO_HORIZONTAL - 0.5 <= 0.5 + 2.5 - 3 
		&& pos_alvo_vertical - 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical - 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL + 0.5 >= -0.5 - 2.5 + 10 
		&& POS_ALVO_HORIZONTAL + 0.5 <= 0.5 + 2.5 + 10 
		&& pos_alvo_vertical + 0.5 >= -0.5 + pos_flecha_2_horizontal && pos_alvo_vertical + 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL + 0.5 >= -0.5 - 2.5 + 10 
		&& POS_ALVO_HORIZONTAL + 0.5 <= 0.5 + 2.5 + 10 
		&& pos_alvo_vertical - 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical - 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL - 0.5 >= -0.5 - 2.5 + 10 
		&& POS_ALVO_HORIZONTAL - 0.5 <= 0.5 + 2.5 + 10 
		&& pos_alvo_vertical + 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical + 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL - 0.5 >= -0.5 - 2.5 + 10 
		&& POS_ALVO_HORIZONTAL - 0.5 <= 0.5 + 2.5 + 10 
		&& pos_alvo_vertical - 0.5 >= -0.5 + pos_flecha_2_horizontal 
		&& pos_alvo_vertical - 0.5 <= 0.5 + pos_flecha_2_horizontal) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}

	//FLECHA_A
	if (POS_ALVO_HORIZONTAL + 0.5 >= -0.5 + pos_flecha_1_horizontal - 2.5 
		&& POS_ALVO_HORIZONTAL + 0.5 <= 0.5 + pos_flecha_1_horizontal + 2.5 
		&& pos_alvo_vertical + 0.5 >= -0.5 - 7.0f && pos_alvo_vertical + 0.5 <= 0.5 - 7.0f) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL + 0.5 >= -0.5 + pos_flecha_1_horizontal - 2.5 
		&& POS_ALVO_HORIZONTAL + 0.5 <= 0.5 + pos_flecha_1_horizontal + 2.5 
		&& pos_alvo_vertical - 0.5 >= -0.5 - 7.0f && pos_alvo_vertical - 0.5 <= 0.5 - 7.0f) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL - 0.5 >= -0.5 + pos_flecha_1_horizontal - 2.5 
		&& POS_ALVO_HORIZONTAL - 0.5 <= 0.5 + pos_flecha_1_horizontal + 2.5 
		&& pos_alvo_vertical + 0.5 >= -0.5 - 7.0f && pos_alvo_vertical + 0.5 <= 0.5 - 7.0f) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
	if (POS_ALVO_HORIZONTAL - 0.5 >= -0.5 + pos_flecha_1_horizontal - 2.5 
		&& POS_ALVO_HORIZONTAL - 0.5 <= 0.5 + pos_flecha_1_horizontal + 2.5 
		&& pos_alvo_vertical - 0.5 >= -0.5 - 7.0f && pos_alvo_vertical - 0.5 <= 0.5 - 7.0f) {
		pontuacao++;
		cout << "Pontuacao: " << pontuacao;
	}
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

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
	//------------------ // BACKGROUND // ---------------------------------------------------------//
	glGenTextures(1, &background);
	glBindTexture(GL_TEXTURE_2D, background); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../textures/ceu_2.png", &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // FLECHA // ---------------------------------------------------------//
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data1 = stbi_load("../textures/flecha.png", &width, &height, &nrChannels, 0);
	if (data1){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data1);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // ALVO // ---------------------------------------------------------//
	glGenTextures(1, &alvo);
	glBindTexture(GL_TEXTURE_2D, alvo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data3 = stbi_load("../textures/alvo.png", &width, &height, &nrChannels, 0);
	if (data3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

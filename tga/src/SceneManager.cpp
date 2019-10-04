#include "SceneManager.h"
//static controllers for mouse and keyboard
static bool keys[1024];
//window attributes
static bool resized;
static GLuint width, height;
//custom global variables
float nuvens = -5.0;
//target location attributes
float pos_horizontal_target = 2;
float pos_vertical_target = -7;
//game state attributes
bool endgame = false;
bool startgame = false;
//projectile location attributes
float tiroA = -20;
float tiroB = 18;
float tiroC = 18;
float tiroD = -20;

int cont = 0;
//background location attributes
float pos_background_a = 0;
float pos_background_b = 0;
int d = 0;

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
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
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

	if (keys[GLFW_KEY_D]){
		if (pos_horizontal_target < 12){
			pos_horizontal_target = pos_horizontal_target + 0.007;
			pos_background_a = pos_background_a + 0.002;
			pos_background_b = pos_background_b + 0.0009;
			d = 0;
		}
	}
		
	if (keys[GLFW_KEY_A]) {
		if (pos_horizontal_target > -12) {
			pos_horizontal_target = pos_horizontal_target - 0.007;
			pos_background_a = pos_background_a - 0.002;
			pos_background_b = pos_background_b - 0.0009;
			d = 1;
		}
	}

	if (keys[GLFW_KEY_W]) {
		if (pos_vertical_target < 4) {
			for (int i = 0; i < 4; i++) {
				pos_vertical_target = pos_vertical_target + 0.005;
			}
		}
	}
	if (keys[GLFW_KEY_S]) {
		if (pos_vertical_target > -7) {
			for (int i = 0; i < 4; i++) {
				pos_vertical_target = pos_vertical_target - 0.005;
			}
		}
	}

	if (keys[GLFW_KEY_ENTER]) {
		endgame = false;
		startgame = false;
	}

	if (keys[GLFW_KEY_SPACE]) {
		if (startgame == false) {
			startgame = true;
			tiroA = -20;
			tiroB = 18;
			tiroC = 18;
			tiroD = -20;
			cont = 0;
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

	//BACKGROUND_A
	glBindTexture(GL_TEXTURE_2D, textureE6);
	modelC = glm::mat4();
	modelC = glm::translate(modelC, glm::vec3(pos_background_a, -3.5f, 0.0f));
	modelC = glm::scale(modelC, glm::vec3(35.0f, 10.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelC));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//BACKGROUND_B
	glBindTexture(GL_TEXTURE_2D, texture);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(pos_background_b, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(32.0f, 27.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//TARGET
	unsigned int es[2] = {textureE1,textureE2};
	glBindTexture(GL_TEXTURE_2D, es[d]);
	model3 = glm::mat4();
	model3 = glm::translate(model3, glm::vec3(pos_horizontal_target, pos_vertical_target, 0.0f));
	model3 = glm::scale(model3, glm::vec3(2.0f, 3.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	//NUVENS
	glBindTexture(GL_TEXTURE_2D, texture2);
	model4 = glm::mat4();
	model4 = glm::translate(model4, glm::vec3(nuvens, 7.0f, 0.0f));
	model4 = glm::scale(model4, glm::vec3(5.0f, 5.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	nuvens = nuvens + 0.001;
	if (nuvens >= 18)
		nuvens = -13;

	if (startgame == false) {
		glBindTexture(GL_TEXTURE_2D, textureE7);
		model8 = glm::mat4();
		model8 = glm::translate(model8, glm::vec3(0.0f, 0.0f, 0.0f));
		model8 = glm::scale(model8, glm::vec3(10.0f, 10.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model8));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	//METEOROS
	if (startgame == true) {
		tiroB = tiroB - 0.004;
		tiroC = tiroC - 0.006;
		glBindTexture(GL_TEXTURE_2D, texture1);
		model2 = glm::mat4();
		model2 = glm::translate(model2, glm::vec3(-3.0f, tiroB, 0.0f));
		model2 = glm::scale(model2, glm::vec3(5.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		glBindTexture(GL_TEXTURE_2D, texture1);
		model2 = glm::mat4();
		model2 = glm::translate(model2, glm::vec3(10.0f, tiroB, 0.0f));
		model2 = glm::scale(model2, glm::vec3(5.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, texture1);
		model2 = glm::mat4();
		model2 = glm::translate(model2, glm::vec3(3.0f, tiroC, 0.0f));
		model2 = glm::scale(model2, glm::vec3(5.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		if (tiroC < -13)
			tiroC = 18;

		glBindTexture(GL_TEXTURE_2D, texture1);
		model2 = glm::mat4();
		model2 = glm::translate(model2, glm::vec3(-10.0f, tiroC, 0.0f));
		model2 = glm::scale(model2, glm::vec3(5.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		if (tiroB < -13)
			tiroB = 18;

		cont = cont + 1;
		if (cont > 10000) {
			tiroA = tiroA + 0.005;
			glBindTexture(GL_TEXTURE_2D, texture3);
			model5 = glm::mat4();
			model5 = glm::translate(model5, glm::vec3(tiroA, -7.0f, 0.0f));
			model5 = glm::scale(model5, glm::vec3(5.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			if (tiroA >= 18)
				tiroA = -13;
		}

		if (cont > 40000) {
			tiroD = tiroD + 0.006;
			glBindTexture(GL_TEXTURE_2D, texture3);
			model5 = glm::mat4();
			model5 = glm::translate(model5, glm::vec3(tiroD, 1.0f, 0.0f));
			model5 = glm::scale(model5, glm::vec3(5.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			if (tiroD >= 18)
				tiroD = -13;
		}



		//COLISAO

		//METEOROS
		// METEORO_B
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 - 3 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 - 3 && pos_vertical_target + 0.5 >= -0.5 + tiroB && pos_vertical_target + 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 - 3 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 - 3 && pos_vertical_target - 0.5 >= -0.5 + tiroB && pos_vertical_target - 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 - 3 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 - 3 && pos_vertical_target + 0.5 >= -0.5 + tiroB && pos_vertical_target + 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 - 3 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 - 3 && pos_vertical_target - 0.5 >= -0.5 + tiroB && pos_vertical_target - 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 + 10 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 + 10 && pos_vertical_target + 0.5 >= -0.5 + tiroB && pos_vertical_target + 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 + 10 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 + 10 && pos_vertical_target - 0.5 >= -0.5 + tiroB && pos_vertical_target - 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 + 10 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 + 10 && pos_vertical_target + 0.5 >= -0.5 + tiroB && pos_vertical_target + 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 + 10 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 + 10 && pos_vertical_target - 0.5 >= -0.5 + tiroB && pos_vertical_target - 0.5 <= 0.5 + tiroB) {
			endgame = true;
		}
		//METEORO_C
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 - 10 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 - 10 && pos_vertical_target + 0.5 >= -0.5 + tiroC && pos_vertical_target + 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 - 10 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 - 10 && pos_vertical_target - 0.5 >= -0.5 + tiroC && pos_vertical_target - 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 - 10 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 - 10 && pos_vertical_target + 0.5 >= -0.5 + tiroC && pos_vertical_target + 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 - 10 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 - 10 && pos_vertical_target - 0.5 >= -0.5 + tiroC && pos_vertical_target - 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 + 3 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 + 3 && pos_vertical_target + 0.5 >= -0.5 + tiroC && pos_vertical_target + 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 - 2.5 + 3 && pos_horizontal_target + 0.5 <= 0.5 + 2.5 + 3 && pos_vertical_target - 0.5 >= -0.5 + tiroC && pos_vertical_target - 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 + 3 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 + 3 && pos_vertical_target + 0.5 >= -0.5 + tiroC && pos_vertical_target + 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 - 2.5 + 3 && pos_horizontal_target - 0.5 <= 0.5 + 2.5 + 3 && pos_vertical_target - 0.5 >= -0.5 + tiroC && pos_vertical_target - 0.5 <= 0.5 + tiroC) {
			endgame = true;
		}
		//METEORO_A
		if (pos_horizontal_target + 0.5 >= -0.5 + tiroA - 2.5 && pos_horizontal_target + 0.5 <= 0.5 + tiroA + 2.5 && pos_vertical_target + 0.5 >= -0.5 - 7.0f && pos_vertical_target + 0.5 <= 0.5 - 7.0f) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 + tiroA - 2.5 && pos_horizontal_target + 0.5 <= 0.5 + tiroA + 2.5 && pos_vertical_target - 0.5 >= -0.5 - 7.0f && pos_vertical_target - 0.5 <= 0.5 - 7.0f) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 + tiroA - 2.5 && pos_horizontal_target - 0.5 <= 0.5 + tiroA + 2.5&& pos_vertical_target + 0.5 >= -0.5 - 7.0f && pos_vertical_target + 0.5 <= 0.5 - 7.0f) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 + tiroA - 2.5 && pos_horizontal_target - 0.5 <= 0.5 + tiroA + 2.5 && pos_vertical_target - 0.5 >= -0.5 - 7.0f && pos_vertical_target - 0.5 <= 0.5 - 7.0f) {
			endgame = true;
		}
		//METEORO_D
		if (pos_horizontal_target + 0.5 >= -0.5 + tiroD - 2.5 && pos_horizontal_target + 0.5 <= 0.5 + tiroD + 2.5 && pos_vertical_target + 0.5 >= -0.5 + 1 && pos_vertical_target + 0.5 <= 0.5 + 1) {
			endgame = true;
		}
		if (pos_horizontal_target + 0.5 >= -0.5 + tiroD - 2.5 && pos_horizontal_target + 0.5 <= 0.5 + tiroD + 2.5 && pos_vertical_target - 0.5 >= -0.5 + 1 && pos_vertical_target - 0.5 <= 0.5 + 1) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 + tiroD - 2.5 && pos_horizontal_target - 0.5 <= 0.5 + tiroD + 2.5&& pos_vertical_target + 0.5 >= -0.5 + 1 && pos_vertical_target + 0.5 <= 0.5 + 1) {
			endgame = true;
		}
		if (pos_horizontal_target - 0.5 >= -0.5 + tiroD - 2.5 && pos_horizontal_target - 0.5 <= 0.5 + tiroD + 2.5 && pos_vertical_target - 0.5 >= -0.5 + 1 && pos_vertical_target - 0.5 <= 0.5 + 1) {
			endgame = true;
		}

	}

	//GAME OVER TELA
	if (endgame == true) {
		glBindTexture(GL_TEXTURE_2D, textureG);
		model6 = glm::mat4();
		model6 = glm::scale(model6, glm::vec3(30.0f, 20.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	//------------------ // BACKGROUND_A // ---------------------------------------------------------//
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../textures/cenario.png", &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

	//------------------ // BACKGROUND_B // ---------------------------------------------------------//
	glGenTextures(1, &textureE6);
	glBindTexture(GL_TEXTURE_2D, textureE6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data20 = stbi_load("../textures/cenario2.png", &width, &height, &nrChannels, 0);
	if (data20) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data20);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data20);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // METEORO // ---------------------------------------------------------//
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data1 = stbi_load("../textures/meteoro.png", &width, &height, &nrChannels, 0);
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


	//------------------ // NUVENS // ---------------------------------------------------------//
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data2 = stbi_load("../textures/nuvens.png", &width, &height, &nrChannels, 0);
	if (data2){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // TARGET // ---------------------------------------------------------//
	glGenTextures(1, &textureE1);
	glBindTexture(GL_TEXTURE_2D, textureE1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data3 = stbi_load("../textures/esquilo1.png", &width, &height, &nrChannels, 0);
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
	// Esquerda
	glGenTextures(1, &textureE2);
	glBindTexture(GL_TEXTURE_2D, textureE2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data33 = stbi_load("../textures/esquiloE.png", &width, &height, &nrChannels, 0);
	if (data33) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data33);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data33);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // START GAME // ---------------------------------------------------------//
	glGenTextures(1, &textureE7);
	glBindTexture(GL_TEXTURE_2D, textureE7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data9 = stbi_load("../textures/start.png", &width, &height, &nrChannels, 0);
	if (data9) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data9);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data9);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//------------------ // GAME OVER // ---------------------------------------------------------//
	glGenTextures(1, &textureG);
	glBindTexture(GL_TEXTURE_2D, textureG);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *data11 = stbi_load("../textures/go.png", &width, &height, &nrChannels, 0);
	if (data11) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data11);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data11);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-3.68f, 5.09002f, -6.0), //mesa 1
	glm::vec3(4.04999f, 5.09002f, -6.0), //mesa 2 centro escenario principal
	glm::vec3(11.7302f, 5.09002f, -6.0), //mesa 3
	glm::vec3(11.6402, 5.09002f, 4.77001f), //mesa 3.2
	glm::vec3(3.81999f, 5.09002f, 4.77001f), //mesa 2.2
	glm::vec3(-3.91f, 5.18003f, 4.77001), //mesa 1.2
	glm::vec3(3.85999f, 5.15003f, 14.2002f), // oficina
};

float vertices[] = {
	// posiciones           // normales           // textura
	-0.5f,-0.5f,-0.5f,      0.0f, 0.0f,-1.0f,     0.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,      0.0f, 0.0f,-1.0f,     1.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,      0.0f, 0.0f,-1.0f,     1.0f, 1.0f,
	 0.5f, 0.5f,-0.5f,      0.0f, 0.0f,-1.0f,     1.0f, 1.0f,
	-0.5f, 0.5f,-0.5f,      0.0f, 0.0f,-1.0f,     0.0f, 1.0f,
	-0.5f,-0.5f,-0.5f,      0.0f, 0.0f,-1.0f,     0.0f, 0.0f,

	-0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
	 0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
	-0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,     -1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
	-0.5f,-0.5f,-0.5f,     -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
	-0.5f,-0.5f,-0.5f,     -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
	-0.5f,-0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,     -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

	 0.5f, 0.5f, 0.5f,      1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,      1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
	 0.5f,-0.5f,-0.5f,      1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
	 0.5f,-0.5f,-0.5f,      1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
	 0.5f,-0.5f, 0.5f,      1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,      1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

	-0.5f,-0.5f,-0.5f,      0.0f,-1.0f, 0.0f,     0.0f, 1.0f,
	 0.5f,-0.5f,-0.5f,      0.0f,-1.0f, 0.0f,     1.0f, 1.0f,
	 0.5f,-0.5f, 0.5f,      0.0f,-1.0f, 0.0f,     1.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,      0.0f,-1.0f, 0.0f,     1.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,      0.0f,-1.0f, 0.0f,     0.0f, 0.0f,
	-0.5f,-0.5f,-0.5f,      0.0f,-1.0f, 0.0f,     0.0f, 1.0f,

	-0.5f, 0.5f,-0.5f,      0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
	 0.5f, 0.5f,-0.5f,      0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
	 0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,      0.0f, 1.0f, 0.0f,     0.0f, 1.0f
};


glm::vec3 Light1 = glm::vec3(0);

//Anim Basicas
//Sol y Nube
float movSol = 0;
bool AnimSol = true;
//Pelota
float movPelota = 0;
bool AnimPelota = false;
//PuertaDercha
float movPuertaD = 0;
bool AnimPuertaD = false;
//PuertaIzquierda
float movPuertaI = 0;
bool AnimPuertaI = false;
//Anim Complejas
////Personaje Puppet
float movPuppet = 0.0f;
float rotBrazoDerP = 0.0f;
float rotBrazoIzqP = 0.0f;
float rotPiernaDerP = 0.0f;
float rotPiernaIzqP = 0.0f;
bool AnimPuppet = false;
bool adelante = true;
////Personaje Chica
float rotBrazoDerC = 0.0f;
float rotBrazoIzqC = 0.0f;
float rotCabezaC = 0.0f;
bool AnimChica = false;
bool lado = true;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion basica", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model ModelosIntegrados((char*)"Models/mdlMapa.obj");
	Model Techo((char*)"Models/mdlTecho.obj");
	Model Sol((char*)"Models/mdlSol.obj");
	Model Nube((char*)"Models/mdlNube.obj");
	Model Bocina((char*)"Models/mdlBocina.obj");
	Model Pelota((char*)"Models/mdlPelota.obj");
	Model Globos((char*)"Models/mdlGlobos.obj");
	Model Sillas((char*)"Models/mdlSillas.obj");
	Model Vaso((char*)"Models/mdlVaso.obj");
	Model PuertaDerecha((char*)"Models/mdlPuertaDerecha.obj");
	Model PuertaIzquierda((char*)"Models/mdlPuertaIzquierda.obj");
	Model Ventilador((char*)"Models/mdlVentilador.obj");
	Model Aspas((char*)"Models/mdlAspas.obj");
	Model Animatronicos((char*)"Models/mdlAnimatronicos.obj");

	//models Personaje Puppet
	Model CabezaP((char*)"Models/mdlCabezaP.obj");
	Model CuelloP((char*)"Models/mdlCuelloP.obj");
	Model CuerpoP((char*)"Models/mdlCuerpoP.obj");
	Model ManoDerechaP((char*)"Models/mdlManoDerechaP.obj");
	Model ManoIzquierdaP((char*)"Models/mdlManoIzquierdaP.obj");
	Model PieDerechoP((char*)"Models/mdlPieDerechoP.obj");
	Model PieIzquierdoP((char*)"Models/mdlPieIzquierdoP.obj");

	//models Personaje Chica
	Model CabezaC((char*)"Models/mdlCabezaC.obj");
	Model CuerpoC((char*)"Models/mdlCuerpoC.obj");
	Model ManoDerechaC((char*)"Models/mdlManoDerechaC.obj");
	Model ManoIzquierdaC((char*)"Models/mdlManoIzquierdaC.obj");
	Model PieDerechoC((char*)"Models/mdlPieDerechoC.obj");
	Model PieIzquierdoC((char*)"Models/mdlPieIzquierdoC.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//===============CUBO CON TEXTURA===============
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	GLuint cubeTextures[17];
	const char* textureFiles[17] = {
	"Models/pizza1.jpg",
	"Models/pizza2.jpg",
	"Models/pizza3.jpg",
	"Models/pizza4.jpg",

	"Models/cajaPizza1.jpg",
	"Models/cajaPizza2.jpg",
	"Models/cajaPizza3.jpg",

	"Models/regalo1.jpg",
	"Models/regalo2.jpg",
	"Models/regalo3.jpg",
	"Models/regalo4.jpg",
	"Models/regalo5.jpg",
	"Models/regalo6.jpg",

	"Models/blanco.jpg",
	"Models/pelos.jpg",
	"Models/poster1.jpg",
	"Models/pantalla.png"
	};

	for (int i = 0; i < 17; i++)
	{
		cubeTextures[i] = SOIL_load_OGL_texture(
			textureFiles[i],
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
		);
		if (cubeTextures[i] == 0)
		{
			std::cout << "Error al cargar textura: [" << i << "]:" << textureFiles[i] << std::endl;
			continue;
		}
		else 
		{
			std::cout << "Textura cargada: " << textureFiles[i] << std::endl;
		}

		glBindTexture(GL_TEXTURE_2D, cubeTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//===============



	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		// POINT LIGHT 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// POINT LIGHT 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// POINT LIGHT 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// POINT LIGHT 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// POINT LIGHT 5
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.032f);

		// POINT LIGHT 6
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.032f);

		//POINT LIGHT 7
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), 0.05f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), 1.0f, 0.8f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.06f, 0.05f, 0.02f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.85f, 0.72f, 0.35f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.25f, 0.22f, 0.12f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.017f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		//Mapa
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		ModelosIntegrados.Draw(lightingShader);
		//Techo
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Techo.Draw(lightingShader);
		//Sol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, movSol, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Sol.Draw(lightingShader);
		//Nube
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -movSol, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Nube.Draw(lightingShader);
		//Bocina
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Bocina.Draw(lightingShader);
		////Pelota
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, movPelota, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Pelota.Draw(lightingShader);
		//Globos
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Globos.Draw(lightingShader);	
		//Sillas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Sillas.Draw(lightingShader);
		//Vaso
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Vaso.Draw(lightingShader);
		//PuertaDerecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, movPuertaD));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		PuertaDerecha.Draw(lightingShader);
		//PuertaIzquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, movPuertaI));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		PuertaIzquierda.Draw(lightingShader);
		//Ventilador
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Ventilador.Draw(lightingShader);
		//Aspas
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Aspas.Draw(lightingShader);
		//Animatronicos
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Animatronicos.Draw(lightingShader);

		//Carga de modelo personaje Puppet
		glm::mat4 modelPuppet = glm::mat4(1);
		modelPuppet = glm::translate(modelPuppet, glm::vec3(movPuppet, 0.0f, 0.0f));
		//CuerpoP
		model = modelPuppet;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		CuerpoP.Draw(lightingShader);
		//CuelloP
		model = modelPuppet;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		CuelloP.Draw(lightingShader);
		//CabezaP
		model = modelPuppet;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		CabezaP.Draw(lightingShader);
		//ManoIzquierdaP
		model = modelPuppet;
		model = glm::rotate(model, glm::radians(rotBrazoIzqP), glm::vec3(0.0f, 0.0f, 1.0f)); // rotar brazo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ManoIzquierdaP.Draw(lightingShader);
		//ManoDerechaP
		model = modelPuppet;
		model = glm::rotate(model, glm::radians(rotBrazoDerP), glm::vec3(0.0f, 0.0f, 1.0f)); // rotación del brazo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ManoDerechaP.Draw(lightingShader);
		//PieIzquierdoP
		model = modelPuppet;
		model = glm::rotate(model, glm::radians(rotPiernaIzqP), glm::vec3(0.0f, 0.0f, 1.0f)); 		// rotar pierna
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PieIzquierdoP.Draw(lightingShader);
		//PieDerechoP
		model = modelPuppet;
		model = glm::rotate(model, glm::radians(rotPiernaDerP), glm::vec3(0.0f, 0.0f, 1.0f)); 		// rotar pierna
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PieDerechoP.Draw(lightingShader);

		//Carga de modelo personaje Chica
		glm::mat4 modelChica = glm::mat4(1);
		modelChica = glm::translate(modelChica, glm::vec3(0.0f, 0.0f, 0.0f));
		//CuerpoC
		model = modelChica;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		CuerpoC.Draw(lightingShader);
		//CabezaC
		model = modelChica;
		model = glm::rotate(model,glm::radians(rotCabezaC),glm::vec3(0.0f, 0.0f, 1.0f)); // rotar cabeza
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		CabezaC.Draw(lightingShader);
		//ManoIzquierdaC
		model = modelChica;
		model = glm::rotate(model,glm::radians(rotBrazoIzqC),glm::vec3(0.0f, 0.0f, 1.0f));// rotar brazor
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		ManoIzquierdaC.Draw(lightingShader);
		//ManoDerechaC
		model = modelChica;
		model = glm::rotate(model,glm::radians(rotBrazoDerC),glm::vec3(0.0f, 0.0f, 1.0f));	// rotar brazo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ManoDerechaC.Draw(lightingShader);
		//PieIzquierdoC
		model = modelChica;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		PieIzquierdoC.Draw(lightingShader);
		//PieDerechoC
		model = modelChica;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		PieDerechoC.Draw(lightingShader);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		// =====================================
		// 7 CUBOS HECHOS CON GEOMETRÍA OPENGL
		// =====================================

		// Posiciones de los cubos
		glm::vec3 cubePositions[] = {
			glm::vec3(-4.5f,  2.7f, 3.5f),
			glm::vec3(4.7f,  2.7f, 3.5f),
			glm::vec3(-3.8f,  2.7f, -6.65f),
			glm::vec3(10.5f,  2.7f, -6.65f),

			glm::vec3(-4.5f, 1.1f,  9.3f), //YA QUEDÓ
			glm::vec3(-4.5f, 0.8f,  9.3f), //YA QUEDÓ
			glm::vec3(-4.5f, 0.5f,  9.3f), //YA QUEDÓ

			glm::vec3(12.5f,  3.0f,  2.8f), //YA QUEDÓ
			glm::vec3(12.0f,  3.0f,  -5.1f), //YA QUEDÓ
			glm::vec3(3.2f,  3.0f,  2.8f), //Medio 
			glm::vec3(3.2f,  3.0f,  -5.1f), //YA QUEDÓ
			glm::vec3(-3.0f,  3.0f,  2.8f), //YA QUEDÓ
			glm::vec3(-4.5f,  3.0f,  -5.1f), //YA QUEDÓ

			glm::vec3(2.9f,  2.37f, 14.5f),
			glm::vec3(3.5f,  0.3f, 17.0f),
			glm::vec3(-8.5f, 4.5f, -6.0f), //YA QUEDÓ
			glm::vec3(16.5f,  4.0f,  0.0f) //YA QUEDÓ
			// x z y
		};

		// Escalas de cada cubo
		glm::vec3 cubeScales[] = {
			glm::vec3(0.5f, 0.04f, 0.8f), // pizza
			glm::vec3(0.5f, 0.04f, 0.8f), // pizza
			glm::vec3(0.5f, 0.04f, 0.8f), // pizza
			glm::vec3(0.5f, 0.04f, 0.8f), // pizza

			glm::vec3(3.0f, 0.35f, 2.0f), // cajaPizza
			glm::vec3(2.0f, 0.35f, 3.0f), // cajaPizza
			glm::vec3(3.0f, 0.35f, 2.0f), // cajaPizza


			glm::vec3(0.7f, 0.7f, 0.7f), // regalo
			glm::vec3(0.7f, 0.7f, 0.7f), // regalo
			glm::vec3(0.7f, 0.7f, 0.7f), // regalo
			glm::vec3(0.7f, 0.7f, 0.7f), // regalo
			glm::vec3(0.7f, 0.7f, 0.7f), // regalo
			glm::vec3(0.7f, 0.7f, 0.7f), // regalo

			glm::vec3(0.4f, 0.03f, 0.5f), // hoja
			glm::vec3(3.0f, 0.05f, 1.5f), // tapete
			glm::vec3(0.05f, 2.0f, 3.0f), // poster
			glm::vec3(0.30f, 3.4f, 6.0f)  // pantalla
			// x y z
		};

		// Dibujar los 7 cubos
		for (GLuint i = 0; i < 17; i++)
		{
			model = glm::mat4(1);

			// Posición
			model = glm::translate(model, cubePositions[i]);

			// Escala
			model = glm::scale(model, cubeScales[i]);

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			//ACTIVAR TEXTURA

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cubeTextures[i]);
			glUniform1i(glGetUniformLocation(lampShader.Program, "texture1"), 0);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	//Activar PuertaDerecha
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		AnimPuertaD = !AnimPuertaD;
	}

	//Activar PuertaIzquierda
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		AnimPuertaI = !AnimPuertaI;
	}
	//Activar Puppet
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		AnimPuppet = !AnimPuppet;
	}
	//Activar Chica
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		AnimChica = !AnimChica;
	}
}
void Animation() {
	//Sol y Nubes
	if (AnimSol)
	{
		movSol += 0.01f;
	}
	else
	{
		movSol -= 0.01f;
	}
	if (movSol >= 0.6f)
	{
		AnimSol = false;
	}
	// límite inferior
	if (movSol <= 0.0f)
	{
		AnimSol = true;
	}

	// Pelota
	if (AnimPelota)
	{
		movPelota += 0.02f;
	}
	else
	{
		movPelota -= 0.03f;
	}

	// límite arriba
	if (movPelota >= 3.0f)
	{
		AnimPelota = false;
	}

	// límite abajo
	if (movPelota <= 0.0f)
	{
		AnimPelota = true;
	}

	//Puerta Derecha
	// abrir
	if (AnimPuertaD && movPuertaD < 3.0f)
	{
		movPuertaD += 0.01f;
	}
	// cerrar
	if (!AnimPuertaD && movPuertaD > 0.0f)
	{
		movPuertaD -= 0.01f;
	}

	//Puerta Derecha
	// abrir
	if (AnimPuertaI && movPuertaI < 3.0f)
	{
		movPuertaI += 0.01f;
	}
	// cerrar
	if (!AnimPuertaI && movPuertaI > 0.0f)
	{
		movPuertaI -= 0.01f;
	}
	// Puppet caminando
	if (AnimPuppet)
	{
		// mover solo hasta x = 20
		if (movPuppet < 20.0f)
		{
			movPuppet += 0.01f;

			// animación de brazos y piernas
			if (adelante)
			{
				rotBrazoDerP += 0.01f;
				rotBrazoIzqP -= 0.01f;

				rotPiernaDerP -= 0.01f;
				rotPiernaIzqP += 0.01f;
			}
			else
			{
				rotBrazoDerP -= 0.01f;
				rotBrazoIzqP += 0.01f;

				rotPiernaDerP += 0.01f;
				rotPiernaIzqP -= 0.01f;
			}

			if (rotBrazoDerP >= 2.0f)
			{
				adelante = false;
			}

			if (rotBrazoDerP <= -2.0f)
			{
				adelante = true;
			}
		}
	}

	// Chica bailando
	if (AnimChica)
	{
		if (lado)
		{
			rotBrazoDerC += 0.03f;
			rotBrazoIzqC += 0.03f;
			rotCabezaC -= 0.02f;
		}
		else
		{
			rotBrazoDerC -= 0.03f;
			rotBrazoIzqC -= 0.03f;
			rotCabezaC += 0.02f;
		}

		// límite
		if (rotBrazoDerC >= 2.0f)
		{
			lado = false;
		}

		if (rotBrazoDerC <= -2.0f)
		{
			lado = true;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
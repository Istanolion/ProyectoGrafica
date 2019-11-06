/*---------------------------------------------------------*/
/*-----------------------Proyecto final-------------------- */
//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

#include "camera.h"
#include "Model.h"

// Other Libs
#include "SOIL2/SOIL2.h"
#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);

void myData(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Texture
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_panda,
				t_cubo,
				t_caja,
				t_caja_brillo;

//For model
bool animacion = false;
float movKit_z = 0.0f;

//Musica
bool GandalfTime = false;


unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	
}

void myData()
{	
	float vertices[] = {
	0

	};
	unsigned int indices[] = {
	0
	};
	
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
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{
	
}

void display(Shader shader, Model cpu1, Model cpu2, Model monitor, Model mesa, Model pisomadera, Model pisometal, Model techo, Model extinguidor)
{
	
	shader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 flor = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);
	

	
	//Dibujo 1a mesa lado derecho
	mesa.Draw(shader);
	//Dibujo 1er monitor
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -0.1f));
	model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-1.7f,0.44f,0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);

	//dibujo 2a mesa lado derecho
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -3.0f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Dibujo 1er monitor
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-1.7f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);

	//dibujo 3a mesa lado derecho
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -6.0f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Dibujo 1er monitor
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-1.7f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);

	//dibujo 4a mesa lado derecho
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -9.0f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Dibujo 1er monitor
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-1.7f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);

	//Dibujo piso de madera
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.0f, -5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(15.0f, 15.0f, 1.0f));
	shader.setMat4("model", model);
	pisomadera.Draw(shader);

	//Dibujo piso de metal
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -1.97f, -5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 15.0f, 1.0f));
	shader.setMat4("model", model);
	pisometal.Draw(shader);

	//Dibujo techo
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 5.0f, -5.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(15.0f, 15.0f, 1.0f));
	shader.setMat4("model", model);
	techo.Draw(shader);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);
	
	Shader modelShader("Shaders/modelLoading.vs", "Shaders/modelLoading.fs");
	// Load models
	Model mesa = ((char*)"Models/Mesa/mesa.obj");
	Model cpu1 = ((char*)"Models/cpu1/CPU1.obj");
	Model cpu2 = ((char*)"Models/cpu2/CPU2.obj");
	Model monitor = ((char*)"Models/Monitor/monitor.obj");

	Model extinguidor = ((char*)"Models/Extinguidor/extinguidor.obj");

	Model pisomadera = ((char *)"Models/Piso/Piso.obj");
	Model pisometal = ((char*)"Models/PisoE/PisoE.obj");
	Model techo = ((char*)"Models/Techo/Techo.obj");

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel);
		display(modelShader,cpu1,cpu2,monitor,mesa, pisomadera, pisometal, techo, extinguidor);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		animacion = true;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		GandalfTime = !GandalfTime;
		if (GandalfTime)
			SoundEngine->play2D("Musica/GandalfSax.mp3", GL_TRUE);
		else
			SoundEngine->stopAllSounds();
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
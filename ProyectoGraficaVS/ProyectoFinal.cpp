/*---------------------------------------------------------*/
/*-----------------------Proyecto final-------------------- */
//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>
#include <windows.h>
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
unsigned int GandalfTexture[71];
int i = 0;

//For model
bool animacion = false;
bool avanzar = false;
bool giro = false;
bool salto = false;
bool baja = false;
float movKit_x = -14.0f;
float movKit_y = -2.0f;
float movGiro_y = 90.0f;

//Musica
bool GandalfTime = false;


float	posY = 0.0f,
scaleX = 0.0f,
scaleY = 0.0f,
scaleZ = 0.0f;

float	incpY = 0.0f,
incsX = 0.0f,
incsY = 0.0f,
incsZ = 0.0f;


#define MAX_FRAMES 9
int i_max_steps = 90;
int i_curr_steps = 0;
int saltos = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float scaleX;
	float scaleY;
	float scaleZ;
	float posY;		//Variable para PosicionY

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void interpolation(void)
{
	incpY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incsX = (KeyFrame[playIndex + 1].scaleX - KeyFrame[playIndex].scaleX) / i_max_steps;
	incsY = (KeyFrame[playIndex + 1].scaleY - KeyFrame[playIndex].scaleY) / i_max_steps;
	incsZ = (KeyFrame[playIndex + 1].scaleZ - KeyFrame[playIndex].scaleZ) / i_max_steps;
}

void InitiateStruct() {
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].scaleX = 0.0f;
	KeyFrame[0].scaleY = 0.0f;
	KeyFrame[0].scaleZ = 0.0f;

	KeyFrame[1].posY = 1.5f;
	KeyFrame[1].scaleX = 0.5f;
	KeyFrame[1].scaleY = 0.5f;
	KeyFrame[1].scaleZ = -0.5f;

	KeyFrame[2].posY = 3.0f;
	KeyFrame[2].scaleX = 0.0f;
	KeyFrame[2].scaleY = 0.0f;
	KeyFrame[2].scaleZ = 0.0f;

	KeyFrame[3].posY = 3.0f;
	KeyFrame[3].scaleX = -0.5f;
	KeyFrame[3].scaleY = -0.5f;
	KeyFrame[3].scaleZ = 0.5f;

	KeyFrame[4].posY = 3.0f;
	KeyFrame[4].scaleX = 0.0f;
	KeyFrame[4].scaleY = 0.0f;
	KeyFrame[4].scaleZ = 0.0f;

	KeyFrame[5].posY = 1.5f;
	KeyFrame[5].scaleX = 0.5f;
	KeyFrame[5].scaleY = 0.5f;
	KeyFrame[5].scaleZ = -0.5f;

	KeyFrame[6].posY = 0.0f;
	KeyFrame[6].scaleX = 0.0f;
	KeyFrame[6].scaleY = 0.0f;
	KeyFrame[6].scaleZ = 0.0f;
	FrameIndex = 5;
}
void resetElements(void)
{
	posY = KeyFrame[0].posY;
	scaleX = KeyFrame[0].scaleX;
	scaleY = KeyFrame[0].scaleY;
	scaleZ = KeyFrame[0].scaleZ;

}
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
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
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

	GandalfTexture[0] = generateTextures("Gandalf/Gandalf0000.png", 1);
	GandalfTexture[1] = generateTextures("Gandalf/Gandalf0001.png", 1);
	GandalfTexture[2] = generateTextures("Gandalf/Gandalf0002.png", 1);
	GandalfTexture[3] = generateTextures("Gandalf/Gandalf0003.png", 1);
	GandalfTexture[4] = generateTextures("Gandalf/Gandalf0004.png", 1);
	GandalfTexture[5] = generateTextures("Gandalf/Gandalf0005.png", 1);
	GandalfTexture[6] = generateTextures("Gandalf/Gandalf0006.png", 1);
	GandalfTexture[7] = generateTextures("Gandalf/Gandalf0007.png", 1);
	GandalfTexture[8] = generateTextures("Gandalf/Gandalf0008.png", 1);
	GandalfTexture[9] = generateTextures("Gandalf/Gandalf0009.png", 1);
	GandalfTexture[10] = generateTextures("Gandalf/Gandalf0010.png", 1);
	GandalfTexture[11] = generateTextures("Gandalf/Gandalf0011.png", 1);
	GandalfTexture[12] = generateTextures("Gandalf/Gandalf0012.png", 1);
	GandalfTexture[13] = generateTextures("Gandalf/Gandalf0013.png", 1);
	GandalfTexture[14] = generateTextures("Gandalf/Gandalf0014.png", 1);
	GandalfTexture[15] = generateTextures("Gandalf/Gandalf0015.png", 1);
	GandalfTexture[16] = generateTextures("Gandalf/Gandalf0016.png", 1);
	GandalfTexture[17] = generateTextures("Gandalf/Gandalf0017.png", 1);
	GandalfTexture[18] = generateTextures("Gandalf/Gandalf0018.png", 1);
	GandalfTexture[19] = generateTextures("Gandalf/Gandalf0019.png", 1);
	GandalfTexture[20] = generateTextures("Gandalf/Gandalf0020.png", 1);
	GandalfTexture[21] = generateTextures("Gandalf/Gandalf0021.png", 1);
	GandalfTexture[22] = generateTextures("Gandalf/Gandalf0022.png", 1);
	GandalfTexture[23] = generateTextures("Gandalf/Gandalf0023.png", 1);
	GandalfTexture[24] = generateTextures("Gandalf/Gandalf0024.png", 1);
	GandalfTexture[25] = generateTextures("Gandalf/Gandalf0025.png", 1);
	GandalfTexture[26] = generateTextures("Gandalf/Gandalf0026.png", 1);
	GandalfTexture[27] = generateTextures("Gandalf/Gandalf0027.png", 1);
	GandalfTexture[28] = generateTextures("Gandalf/Gandalf0028.png", 1);
	GandalfTexture[29] = generateTextures("Gandalf/Gandalf0029.png", 1);
	GandalfTexture[30] = generateTextures("Gandalf/Gandalf0030.png", 1);
	GandalfTexture[31] = generateTextures("Gandalf/Gandalf0031.png", 1);
	GandalfTexture[32] = generateTextures("Gandalf/Gandalf0032.png", 1);
	GandalfTexture[33] = generateTextures("Gandalf/Gandalf0033.png", 1);
	GandalfTexture[34] = generateTextures("Gandalf/Gandalf0034.png", 1);
	GandalfTexture[35] = generateTextures("Gandalf/Gandalf0035.png", 1);
	GandalfTexture[36] = generateTextures("Gandalf/Gandalf0036.png", 1);
	GandalfTexture[37] = generateTextures("Gandalf/Gandalf0037.png", 1);
	GandalfTexture[38] = generateTextures("Gandalf/Gandalf0038.png", 1);
	GandalfTexture[39] = generateTextures("Gandalf/Gandalf0039.png", 1);
	GandalfTexture[40] = generateTextures("Gandalf/Gandalf0040.png", 1);
	GandalfTexture[41] = generateTextures("Gandalf/Gandalf0041.png", 1);
	GandalfTexture[42] = generateTextures("Gandalf/Gandalf0042.png", 1);
	GandalfTexture[43] = generateTextures("Gandalf/Gandalf0043.png", 1);
	GandalfTexture[44] = generateTextures("Gandalf/Gandalf0044.png", 1);
	GandalfTexture[45] = generateTextures("Gandalf/Gandalf0045.png", 1);
	GandalfTexture[46] = generateTextures("Gandalf/Gandalf0046.png", 1);
	GandalfTexture[47] = generateTextures("Gandalf/Gandalf0047.png", 1);
	GandalfTexture[48] = generateTextures("Gandalf/Gandalf0048.png", 1);
	GandalfTexture[49] = generateTextures("Gandalf/Gandalf0049.png", 1);
	GandalfTexture[50] = generateTextures("Gandalf/Gandalf0050.png", 1);
	GandalfTexture[51] = generateTextures("Gandalf/Gandalf0051.png", 1);
	GandalfTexture[52] = generateTextures("Gandalf/Gandalf0052.png", 1);
	GandalfTexture[53] = generateTextures("Gandalf/Gandalf0053.png", 1);
	GandalfTexture[54] = generateTextures("Gandalf/Gandalf0054.png", 1);
	GandalfTexture[55] = generateTextures("Gandalf/Gandalf0055.png", 1);
	GandalfTexture[56] = generateTextures("Gandalf/Gandalf0056.png", 1);
	GandalfTexture[57] = generateTextures("Gandalf/Gandalf0057.png", 1);
	GandalfTexture[58] = generateTextures("Gandalf/Gandalf0058.png", 1);
	GandalfTexture[59] = generateTextures("Gandalf/Gandalf0059.png", 1);
	GandalfTexture[60] = generateTextures("Gandalf/Gandalf0060.png", 1);
	GandalfTexture[61] = generateTextures("Gandalf/Gandalf0061.png", 1);
	GandalfTexture[62] = generateTextures("Gandalf/Gandalf0062.png", 1);
	GandalfTexture[63] = generateTextures("Gandalf/Gandalf0063.png", 1);
	GandalfTexture[64] = generateTextures("Gandalf/Gandalf0064.png", 1);
	GandalfTexture[65] = generateTextures("Gandalf/Gandalf0065.png", 1);
	GandalfTexture[66] = generateTextures("Gandalf/Gandalf0066.png", 1);
	GandalfTexture[67] = generateTextures("Gandalf/Gandalf0067.png", 1);
	GandalfTexture[68] = generateTextures("Gandalf/Gandalf0068.png", 1);
	GandalfTexture[69] = generateTextures("Gandalf/Gandalf0069.png", 1);
	GandalfTexture[70] = generateTextures("Gandalf/Gandalf0070.png", 1);

}

void myData()
{	
	float vertices[] = {
		//vertex	//textures 
	// positions          // texture coords
		-2.0f, -2.0f, 0.1f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 2.0f, -2.0f, 0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f, 0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-2.0f,  2.0f, 0.1f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posY += incpY;
			scaleX += incsX;
			scaleY += incsY;
			scaleZ += incsZ;
			i_curr_steps++;
		}

	}
	if (animacion) {
		if (!avanzar && !giro && !salto && !baja) {
			movKit_x += 0.0425f;
			if (movKit_x >= -10.0f)
				giro = true;
		}
		else if (!avanzar && giro && !salto && !baja) {
			movKit_x += 0.0425f;
			movGiro_y += 20.0f;
			if (movGiro_y >= 420.0f && movKit_x >= -3.75f) 
				salto = true;
			
		}
		else if (!avanzar && giro && salto && !baja) {
			movKit_x += 0.0425f;
			movKit_y += 0.05f;
			if (movKit_y >= -1.5f ) {
				baja = true;
				salto = false;
			}
		}
		else if (!avanzar && giro && !salto && baja) {
			movKit_x += 0.0425f;
			movKit_y -= 0.05f;
			if (movKit_y <= -2.0f ) {
				baja = false;
				salto = true;
				saltos++;
			}
		}
		if (saltos == 6) {
			animacion = false;
			SoundEngine->stopAllSounds();
		}
	}
	
}

void display(Shader shader, Model cpu1, Model cpu2, Model monitor, Model mesa, Model pisomadera, Model pisometal, Model techo, Model extinguidor, Model mesa3, Model mouse, 
	Model teclado, Model muro, Model silla, Model pizarron, Model padoru, Model mesaProf, Model soccer, Model Shield, Model saber)


{
	
	shader.use();
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

	//shader.setVec3("pointLight.position", lightPosition);
	shader.setVec3("pointLight.position", glm::vec3(2.0f, 4.0f, 3.0f));


	shader.setVec3("pointLight.ambient", glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight.diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setVec3("pointLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight.constant", 1.0f);
	shader.setFloat("pointLight.linear", 0.9f);
	shader.setFloat("pointLight.quadratic", 0.032f);



	shader.setFloat("material_shininess", 32.0f);
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
	
	//Keyframe Animation of Ball
	model = glm::translate(tmp, glm::vec3(-10.0f, 3.0f - posY, 3.0f));
	model = glm::scale(model, glm::vec3(1.0f + scaleX, 1.0f + scaleY, 1.0f + scaleZ));
	shader.setMat4("model", model);
	soccer.Draw(shader);

	//Shield
	model = glm::translate(tmp, glm::vec3(-5.0f, 3.0f , 3.0f));
	model = glm::scale(model, glm::vec3(1.0f , 1.0f , 1.0f ));
	shader.setMat4("model", model);
	Shield.Draw(shader);

	model = glm::mat4(1.0f);

	//Se dibuja el pizarron
	model = glm::translate(tmp, glm::vec3(-6.0f, 2.0f, -16.9f));
	shader.setMat4("model", model);
	pizarron.Draw(shader);

	// Se dibuja la mesa del profe
	model = glm::translate(tmp, glm::vec3(-0.8f, 0.0f, -11.2f));
	shader.setMat4("model", model);
	mesaProf.Draw(shader);
	//Textura Gandalf;
	if (GandalfTime) {
		model = glm::translate(tmp, glm::vec3(-6.0f, 2.0f, -16.9f));
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, GandalfTexture[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		if (i < 70) {
			//Sleep(50);
			i++;

		}
		else
			i = 0;
	}
	

	model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	//Dibujo 1a mesa lado derecho
	mesa.Draw(shader);
	//Dibujo 1er equipo computo
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.0f, -0.1f));
	model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor	
	model = glm::translate(tmp, glm::vec3(-1.7f,0.44f,0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU		
	model = glm::translate(tmp, glm::vec3(-2.37f, 0.05f, 0.4f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-1.9f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-2.8f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor	
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);  //CPU		
	model = glm::translate(tmp, glm::vec3(-1.05f, 0.05f, 0.4f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-0.55f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-1.35f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);	//Monitor
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);		//CPU
	model = glm::translate(tmp, glm::vec3(0.40f, 0.05f, 0.4f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(0.90f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model); 
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(0.40f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader);	//Monitor
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);		//CPU
	model = glm::translate(tmp, glm::vec3(1.75f, 0.05f, 0.4f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(2.25f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(2.15f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//dibujo 2a mesa lado derecho
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -3.0f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Dibujo 1er equipo de computo
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-1.7f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);		//CPU
	model = glm::translate(tmp, glm::vec3(-2.37f, 0.05f, -2.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-1.9f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-2.8f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//sila
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-1.05f, 0.05f, -2.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-0.55f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-1.35f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(0.40f, 0.05f, -2.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(0.90f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(0.40f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(1.75f, 0.05f, -2.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(2.25f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(2.15f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//dibujo 3a mesa lado derecho
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -6.0f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Dibujo 1er monitor
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-1.7f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-2.37f, 0.05f, -5.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-1.9f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-2.8f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//sila
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-1.05f, 0.05f, -5.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-0.55f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-1.35f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(0.40f, 0.05f, -5.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(0.90f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(0.40f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(1.75f, 0.05f, -5.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(2.25f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(2.15f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//dibujo 4a mesa lado derecho
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -9.0f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Dibujo 1er equipo computp
	model = glm::translate(tmp, glm::vec3(-2.4f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-1.7f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-2.37f, 0.05f, -8.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-1.9f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-2.8f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//sila
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-0.95f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-0.3f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-1.05f, 0.05f, -8.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-0.55f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-1.35f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(0.45f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(1.1f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(0.40f, 0.05f, -8.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(0.90f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(0.40f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 4o equipo de computo
	model = glm::translate(tmp, glm::vec3(1.9f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(2.6f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(1.75f, 0.05f, -8.6f));
	model = glm::rotate(model,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(2.25f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(2.15f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//Dibujo piso de madera/lozeta
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.0f, -5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(45.0f, 45.0f, 1.0f));
	shader.setMat4("model", model);
	pisomadera.Draw(shader);

	//Dibujo 1a mesa lado izq
	model = glm::translate(tmp, glm::vec3(-12.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	mesa3.Draw(shader);
	//Dibujo 1er equipo computo
	model = glm::translate(tmp, glm::vec3(-9.8f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-10.6f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-10.0f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-9.3f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-9.8f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-11.6f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-12.6f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-11.8f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-11.2f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-11.8f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(-13.6f, 0.03f, -0.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-14.6f, 0.44f, 0.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-13.8f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-13.2f, 0.05f, 0.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-13.8f, -0.05f, -0.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//Dibujo 2a mesa lado izq
	model = glm::translate(tmp, glm::vec3(-12.0f, 0.0f, -3.0f));
	shader.setMat4("model", model);
	mesa3.Draw(shader);
	//Dibujo 1er equipo computo
	model = glm::translate(tmp, glm::vec3(-9.8f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-10.6f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-10.0f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-9.3f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-9.8f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-11.6f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-12.6f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-11.8f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-11.2f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-11.8f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(-13.6f, 0.03f, -3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-14.6f, 0.44f, -3.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-13.8f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-13.2f, 0.05f, -2.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-13.8f, -0.05f, -3.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//Dibujo 3a mesa lado izq
	model = glm::translate(tmp, glm::vec3(-12.0f, 0.0f, -6.0f));
	shader.setMat4("model", model);
	mesa3.Draw(shader);
	//Dibujo 1er equipo computo
	model = glm::translate(tmp, glm::vec3(-9.8f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-10.6f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-10.0f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-9.3f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-9.8f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-11.6f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-12.6f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-11.8f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-11.2f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-11.8f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(-13.6f, 0.03f, -6.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-14.6f, 0.44f, -6.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-13.8f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-13.2f, 0.05f, -5.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-13.8f, -0.05f, -6.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//Dibujo 2a mesa lado izq
	model = glm::translate(tmp, glm::vec3(-12.0f, 0.0f, -9.0f));
	shader.setMat4("model", model);
	mesa3.Draw(shader);
	//Dibujo 1er equipo computo
	model = glm::translate(tmp, glm::vec3(-9.8f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-10.6f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-10.0f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-9.3f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-9.8f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 2do equipo de computo
	model = glm::translate(tmp, glm::vec3(-11.6f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-12.6f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu1.Draw(shader);  //CPU
	model = glm::translate(tmp, glm::vec3(-11.8f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-11.2f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-11.8f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla
	//Dibujo de 3er equipo de computo
	model = glm::translate(tmp, glm::vec3(-13.6f, 0.03f, -9.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	monitor.Draw(shader); //Monitor
	model = glm::translate(tmp, glm::vec3(-14.6f, 0.44f, -9.0f));
	shader.setMat4("model", model);
	cpu2.Draw(shader);  //CPU  
	model = glm::translate(tmp, glm::vec3(-13.8f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 1.0f));
	shader.setMat4("model", model);
	teclado.Draw(shader);	//Teclado
	model = glm::translate(tmp, glm::vec3(-13.2f, 0.05f, -8.6f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	mouse.Draw(shader);		//Mouse
	model = glm::translate(tmp, glm::vec3(-13.8f, -0.05f, -9.4f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	silla.Draw(shader);		//Silla

	//Dibujo piso de metal
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -1.97f, -5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 25.0f, 1.0f));
	shader.setMat4("model", model);
	pisometal.Draw(shader);

	//Dibujo techo
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 5.0f, -5.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(60.0f, 40.0f, 1.0f));
	shader.setMat4("model", model);
	techo.Draw(shader);

	//Muro trasero
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.2f, 1.5f, 6.0f));
	model = glm::scale(model, glm::vec3(160.0f, 7.0f, 1.0f));
	shader.setMat4("model", model);
	muro.Draw(shader);

	//Muro lateral izquierda
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 1.5f, -7.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(160.0f, 7.0f, 1.0f));
	shader.setMat4("model", model);
	muro.Draw(shader);

	//Segundo Muro lateral izquierda
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-33.0f, 1.5f, -7.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(160.0f, 7.0f, 1.0f));
	shader.setMat4("model", model);
	muro.Draw(shader);

	//Muro lateral derecha
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.2f, 1.5f, -7.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(160.0f, 7.0f, 1.0f));
	shader.setMat4("model", model);
	muro.Draw(shader);

	//Segundo Muro lateral derecha
	model = glm::translate(glm::mat4(1.0f), glm::vec3(21.4f, 1.5f, -7.8f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(160.0f, 7.0f, 1.0f));
	shader.setMat4("model", model);
	muro.Draw(shader);

	//Muro delantero
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.2f, 1.5f, -17.0f));
	model = glm::scale(model, glm::vec3(160.0f, 7.0f, 1.0f));
	shader.setMat4("model", model);
	muro.Draw(shader);

	//Extintor
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.05f, 1.5f, 5.9f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	extinguidor.Draw(shader);

/*	//Garrafon
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.05f, 5.9f));
	shader.setMat4("model", model);
	garrafon.Draw(shader);		*/

	//Padoru
	model = glm::translate(glm::mat4(1.0f), glm::vec3(movKit_x, movKit_y, -15.0f));
	model = glm::rotate(model, glm::radians(movGiro_y), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	padoru.Draw(shader);

	//Modelos Fate
	//Saber
	model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.1f, -15.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
	shader.setMat4("model", model);
	saber.Draw(shader);

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

	InitiateStruct();
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	// Load models
	Model soccer = ((char*)"Models/Soccer/soccer.obj");
	Model mesa = ((char*)"Models/Mesa/mesa.obj");
	Model mesa3 = ((char*)"Models/Mesa/mesa3H.obj");
	Model mesaProf= ((char*)"Models/Mesa/MesaProfe.obj");
	Model cpu1 = ((char*)"Models/cpu1/CPU1.obj");
	Model cpu2 = ((char*)"Models/cpu2/CPU2.obj");
	Model monitor = ((char*)"Models/Monitor/monitor.obj");
	Model mouse = ((char*)"Models/Mouse/mouse.obj");
	Model teclado = ((char*)"Models/Teclado/keyboard.obj");

	Model extinguidor = ((char*)"Models/Extinguidor/extinguidor.obj");
//	Model garrafon = ((char*)"Models/Garrafon/garrafon1.obj");
	Model silla= ((char*)"Models/Silla/silla.obj");
	Model pisomadera = ((char *)"Models/Piso/Piso.obj");
	Model pisometal = ((char*)"Models/PisoE/PisoE.obj");
	Model techo = ((char*)"Models/Techo/Techo.obj");
	Model muro = ((char*)"Models/Muros/Muro.obj");
	Model Pizarron = ((char*)"Models/Pizarron/pizarron.obj");
	Model padoru = ((char*)"Models/Padoru/padoru.obj");
	
	
	//Fate Models
	Model saber = ((char*)"Models/SaberLily/SaberLily1.obj");
	

	//Lord of the rings Models
	Model GondorShield = ((char*)"Models/LotR/isengard.obj");
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

		display(modelShader,cpu1,cpu2,monitor,mesa, pisomadera, pisometal, techo, extinguidor, mesa3, mouse, teclado, muro, silla, Pizarron, padoru, mesaProf, soccer, 
			GondorShield, saber);





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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		resetElements();
		//First Interpolation				
		interpolation();
		play = true;
		playIndex = 0;
		i_curr_steps = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS and !GandalfTime) {
		GandalfTime = true;
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("Musica/GandalfSax.mp3", GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS and GandalfTime) {
		SoundEngine->stopAllSounds();
		GandalfTime = false;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS and !animacion) {
		animacion = true;
		SoundEngine->play2D("Musica/Padoru.mp3", GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		
		movKit_x = -14.0f;
		movGiro_y = 90.0f;
		saltos = 0;
		movKit_y = -2.0f;
		animacion = false;
		giro = false;
		salto = false;
		baja = false;

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
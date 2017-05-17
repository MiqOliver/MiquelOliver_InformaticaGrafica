//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
//SOIL
#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Time.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Object.h"
#include <iostream>

using namespace std;
using namespace glm;

#pragma region global variables

const GLint WIDTH = 800, HEIGHT = 600;
int Numbuffer = 1;
float text_mixer = 0.0;
float rotation_speed = 1.0f;


mat4 model;
mat4 auxModel;
mat4 reset;

bool keys[1024];

Camera camera(
	vec3(2.0f, 2.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	0.05f, 25.0f
	);

#pragma endregion

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() {

#pragma region Init
	//initGLFW
	//TODO
	GLFWwindow* window;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	//TODO
	window = glfwCreateWindow(WIDTH, HEIGHT, "Window", nullptr, nullptr);

	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//set GLEW and inicializate
	//TODO
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al iniciar glew" << endl;
		glfwTerminate();
		return NULL;
	}
	//set function when callback
	//TODO
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//set windows and viewport
	//TODO
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	//fondo
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0);

	//cargamos los shader
	Shader shader("./src/3D_Vertex.vertexshader", "./src/3D_Fragment.fragmentshader");
	shader.USE();

#pragma endregion

#pragma region Buffers

	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	// Crear los VBO, VAO y EBO
	GLuint VBO, VAO; /*EBO;*/

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(Numbuffer, &VAO);
	glGenBuffers(Numbuffer, &VBO);
	//glGenBuffers(Numbuffer, &EBO);

	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubesPositionBuffer), CubesPositionBuffer, GL_STATIC_DRAW);

	//Establecer las propiedades de los vertices

	GLint posAttrib = glGetAttribLocation(shader.Program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	//GLint colAttrib = glGetAttribLocation(shader.Program, "color");
	//glEnableVertexAttribArray(colAttrib);
	//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shader.Program, "texCoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//liberar el buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#pragma endregion

#pragma region Textures

	//Textures
	GLuint textures[2];
	glGenTextures(2, textures);
	int width, height;
	unsigned char* image;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	image = SOIL_load_image("./src/kimi_no_na_wa_1.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shader.Program, "texture1"), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	image = SOIL_load_image("./src/kimi_no_na_wa_2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shader.Program, "texture2"), 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#pragma endregion	

	GLint uniModel = glGetUniformLocation(shader.Program, "model");
	GLint uniAuxModel = glGetUniformLocation(shader.Program, "auxModel");

	//Uniforms
	//GLint uniTrans = glGetUniformLocation(shader.Program, "trans");
	GLint mixer = glGetUniformLocation(shader.Program, "mixer");
	GLint uniView = glGetUniformLocation(shader.Program, "view");
	GLuint control = glGetUniformLocation(shader.Program, "control");

	float time_start = Time.GetTime();
	float time_now;
	float time;

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		//Update time
		Time.Update();
		time_now = Time.GetTime();
		time = time_now - time_start;

		glEnable(GL_DEPTH_TEST);
		camera.DoMovement(window, keys);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwSetKeyCallback(window, key_callback);
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 1.0);
		
		//establecer el shader
		shader.USE();

		glUniform1f(mixer, text_mixer);

		glUniformMatrix4fv(uniView, 1, GL_FALSE, value_ptr(camera.LookAt()));

		mat4 proj = perspective(radians(camera.GetFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		GLint uniProj = glGetUniformLocation(shader.Program, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, value_ptr(proj));

		//pitar el VAO
		glBindVertexArray(VAO); {
			//original

			glUniform1i(control, 1);

			glUniformMatrix4fv(uniModel, 1, GL_FALSE, value_ptr(model));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Others


			glUniform1i(control, 0);

			auxModel = translate(auxModel, vec3(-1.5f, -7.7f, 2.0f));
			auxModel = rotate(auxModel, radians(abs(sin(Time.GetTime())) * 360), vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniAuxModel, 1, GL_FALSE, value_ptr(auxModel));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			auxModel = reset;

			auxModel = translate(auxModel, vec3(-0.75f, 5.3f, 0.75f));
			auxModel = rotate(auxModel, radians(-abs(sin(Time.GetTime())) * 360), vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniAuxModel, 1, GL_FALSE, value_ptr(auxModel));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			auxModel = reset;

			auxModel = translate(auxModel, vec3(-3.5f, 0.7f, -2.0f));
			auxModel = rotate(auxModel, radians(abs(cos(Time.GetTime())) * 360), vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniAuxModel, 1, GL_FALSE, value_ptr(auxModel));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			auxModel = reset;

			auxModel = translate(auxModel, vec3(2.5f, -1.0f, -8.3f));
			auxModel = rotate(auxModel, radians(-abs(cos(Time.GetTime())) * 360), vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniAuxModel, 1, GL_FALSE, value_ptr(auxModel));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			auxModel = reset;

			auxModel = translate(auxModel, vec3(0.5f, 3.2f, -10.5f));
			auxModel = rotate(auxModel, radians(abs(sin(Time.GetTime())) * 360), vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniAuxModel, 1, GL_FALSE, value_ptr(auxModel));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			auxModel = reset;
		}
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

#pragma region freeMemory

	// liberar la memoria de los VAO, EBO y VBO
	glDeleteTextures(2, textures);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

#pragma endregion

	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	//cuando se pulsa una tecla escape cerramos la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		cout << "Exit" << endl;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		text_mixer = 0.0;
	}	
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		text_mixer = 1.0;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		model = rotate(model, radians(-rotation_speed * Time.DeltaTime()), vec3(1.0f, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		model = rotate(model, radians(rotation_speed * Time.DeltaTime()), vec3(1.0f, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
		model = rotate(model, radians(-rotation_speed * Time.DeltaTime()), vec3(0.0f, 0.0f, 1.0f));
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
		model = rotate(model, radians(rotation_speed * Time.DeltaTime()), vec3(0.0f, 0.0f, 1.0f));
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.MouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.MouseMove(window, xpos, ypos);
}

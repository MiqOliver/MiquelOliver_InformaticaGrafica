#pragma once

#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../src/Time.hpp"

using namespace glm;

class Camera
{
private:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraRight;
	vec3 cameraUp;
	GLfloat LastMx;
	GLfloat LastMy;
	GLfloat Sensitivity;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW;
	GLfloat FOV;

public:
	Camera(vec3, vec3, GLfloat, GLfloat);
	void DoMovement(GLFWwindow*, bool*);
	void MouseMove(GLFWwindow*, double, double);
	void MouseScroll(double);
	mat4 LookAt();
	GLfloat GetFOV();
};

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov) {
	cameraPos = position;
	cameraFront = normalize(position - direction);
	Sensitivity = sensitivity; //2 horas perdidas. s, no S
	FOV = fov;

	cameraRight = cross(cameraFront, vec3(0.0f, 0.0f, 1.0f));
	cameraUp = cross(cameraRight, cameraFront);

	firstMouse = true;
	YAW = -90.0f;
	PITCH = 0.0f;
}

void Camera::DoMovement(GLFWwindow* window, bool* keys) {
	GLfloat cameraSpeed = 0.001f * Time.DeltaTime();
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= cameraRight * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += cameraRight * cameraSpeed;
}

void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}
	
	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;
	LastMx = xpos;
	LastMy = ypos;

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	YAW += xoffset;
	YAW = mod(YAW, 360.0f);
	PITCH += yoffset;

	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;

	cameraFront.x = cos(radians(YAW) * sin(radians(PITCH)));
	cameraFront.y = sin(radians(PITCH));
	cameraFront.z = sin(radians(YAW) * sin(radians(PITCH)));
	cameraFront = normalize(cameraFront);
}

void Camera::MouseScroll(double yScroll) {
	if (FOV >= 45 && FOV <= 110)
		FOV -= yScroll * Time.DeltaTime();
	if (FOV <= 45)		FOV = 45.0f;
	if (FOV >= 110)		FOV = 110.0f;
}

mat4 Camera::LookAt() {
	return lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

inline GLfloat Camera::GetFOV() { return FOV; }
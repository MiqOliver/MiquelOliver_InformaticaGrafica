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
	cameraFront = normalize(direction);
	Sensitivity = sensitivity;
	FOV = fov;

	firstMouse = true;
}

void Camera::DoMovement(GLFWwindow* window, bool* keys) {
	GLfloat cameraSpeed = Time.DeltaTime() * Sensitivity * 0.25f;
	if (keys[GLFW_KEY_W])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
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

	glm::vec3 aux;
	aux.x = sin(radians(YAW)) * cos(radians(PITCH));
	aux.y = sin(radians(PITCH));
	aux.z = cos(radians(YAW)) * cos(radians(PITCH));
	cameraFront = normalize(aux);
}

void Camera::MouseScroll(double yScroll) {
	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= yScroll * Time.DeltaTime() * Sensitivity * 10;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;
}

mat4 Camera::LookAt() {
	vec3 worldUp{ 0.0f, 1.0f, 0.0f };

	vec3 cameraRight = normalize(cross(cameraFront, worldUp));
	cameraUp = cross(cameraFront, cameraRight);

	mat4 VecMat;
	VecMat[0][0] = cameraRight.x;
	VecMat[1][0] = cameraRight.y;
	VecMat[2][0] = cameraRight.z;

	VecMat[0][1] = cameraUp.x;
	VecMat[1][1] = cameraUp.y;
	VecMat[2][1] = cameraUp.z;

	VecMat[0][2] = cameraFront.x;
	VecMat[1][2] = cameraFront.y;
	VecMat[2][2] = cameraFront.z;

	mat4 MatPos;
	MatPos[3][0] = -cameraPos.x;
	MatPos[3][1] = -cameraPos.y;
	MatPos[3][2] = -cameraPos.z;

	mat4 ResMat = VecMat * MatPos;
	return ResMat;
}

inline GLfloat Camera::GetFOV() { return FOV; }
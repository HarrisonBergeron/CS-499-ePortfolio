#include "Camera.h"

Camera::Camera()
	: pos(glm::vec3(0.0f, 0.0f, 0.0f)),
	yaw(0.0f),
	pitch(0.0f)
{
}

void Camera::setPos(glm::vec3 pos)
{
	this->pos = pos;
}

void Camera::setYaw(float yaw)
{
	this->yaw = yaw;
}

void Camera::setPitch(float pitch)
{
	this->pitch = pitch;
}

glm::vec3 Camera::getPos()
{
	return pos;
}

float Camera::getYaw()
{
	return yaw;
}

float Camera::getPitch()
{
	return pitch;
}

void Camera::moveForward(float amt)
{
	pos += forward() * amt;
}

void Camera::moveRight(float amt)
{
	pos += right() * amt;
}

void Camera::moveUp(float amt)
{
	pos += up() * amt;
}

void Camera::rotateYaw(float amt)
{
	yaw += amt;
}

void Camera::rotatePitch(float amt)
{
	pitch += amt;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(pos, pos + front(), up());
}

glm::vec3 Camera::forward()
{
	return glm::vec3(glm::rotate(yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

glm::vec3 Camera::front()
{
	return glm::vec3(glm::rotate(yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(pitch, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

glm::vec3 Camera::right()
{
	return glm::vec3(glm::rotate(yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(pitch, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
}

glm::vec3 Camera::up()
{
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

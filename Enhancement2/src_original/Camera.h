#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();

	void setPos(glm::vec3 pos);
	void setYaw(float yaw);
	void setPitch(float pitch);

	glm::vec3 getPos();
	float getYaw();
	float getPitch();

	void moveForward(float amt);
	void moveRight(float amt);
	void moveUp(float amt);
	void rotateYaw(float amt);
	void rotatePitch(float amt);

	glm::mat4 getViewMatrix();
private:
	glm::vec3 forward(); // Get camera forward look vec
	glm::vec3 front(); // Get camera forward look vec with pitch taken into account
	glm::vec3 right(); // Get camera right look vec
	glm::vec3 up(); // Get camera up vec (always pointing in positive y)

	glm::vec3 pos; // Camera position
	float yaw; // Left-right rotation
	float pitch; // Up down rotation
};
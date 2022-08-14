/* Camera.cpp
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#include "Camera.h"

Camera::Camera()
    : pos_(glm::vec3(0.0f, 0.0f, 0.0f)),
    yaw_(0.0f),
    pitch_(0.0f)
{
}

void Camera::SetPos(glm::vec3 pos)
{
    this->pos_ = pos;
}

void Camera::SetYaw(float yaw)
{
    this->yaw_ = yaw;
}

void Camera::SetPitch(float pitch)
{
    this->pitch_ = pitch;
}

glm::vec3 Camera::GetPos()
{
    return pos_;
}

float Camera::GetYaw()
{
    return yaw_;
}

float Camera::GetPitch()
{
    return pitch_;
}

void Camera::MoveForward(float amt)
{
    pos_ += Forward() * amt;
}

void Camera::MoveRight(float amt)
{
    pos_ += Right() * amt;
}

void Camera::MoveUp(float amt)
{
    pos_ += Up() * amt;
}

void Camera::RotateYaw(float amt)
{
    yaw_ += amt;
}

void Camera::RotatePitch(float amt)
{
    pitch_ += amt;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(pos_, pos_ + Front(), Up());
}

glm::vec3 Camera::Forward()
{
    return glm::vec3(glm::rotate(yaw_, glm::vec3(0.0f, 1.0f, 0.0f)) * 
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

glm::vec3 Camera::Front()
{
    return glm::vec3(glm::rotate(yaw_, glm::vec3(0.0f, 1.0f, 0.0f)) * 
        glm::rotate(pitch_, glm::vec3(1.0f, 0.0f, 0.0f)) * 
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

glm::vec3 Camera::Right()
{
    return glm::vec3(glm::rotate(yaw_, glm::vec3(0.0f, 1.0f, 0.0f)) * 
        glm::rotate(pitch_, glm::vec3(1.0f, 0.0f, 0.0f)) * 
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
}

glm::vec3 Camera::Up()
{
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

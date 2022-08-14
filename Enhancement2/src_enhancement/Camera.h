/* Camera.h
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* The Camera class represents a movable view in 3D space. It has an
 * orientation and a position. This class was included to make it easier
 * for the programmer or the user to navigate a 3D scene. */
class Camera
{
public:
    Camera();

    void SetPos(glm::vec3 pos);
    void SetYaw(float yaw);
    void SetPitch(float pitch);

    glm::vec3 GetPos();
    float GetYaw();
    float GetPitch();

    void MoveForward(float amt);
    void MoveRight(float amt);
    void MoveUp(float amt);
    void RotateYaw(float amt);
    void RotatePitch(float amt);

    glm::mat4 GetViewMatrix();
private:
    // Get camera forward look vec
    glm::vec3 Forward();
    // Get camera forward look vec with pitch taken into account
    glm::vec3 Front();
    // Get camera right look vec
    glm::vec3 Right();
    // Get camera up vec (always pointing in positive y)
    glm::vec3 Up();

    glm::vec3 pos_; // Camera position
    float yaw_; // Left-right rotation
    float pitch_; // Up down rotation
};

#endif // CAMERA_H
/* OpenGLApp.h
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#ifndef OPENGL_APP_H
#define OPENGL_APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>

#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Object.h"

/* The OpenGLApp class handles the window, input, and event loop for a 3D
 * OpenGL application. It also provides an environment for navigation of a
 * 3D scene. This class was included to improve modularity and make it possible
 * to reuse functionality for another OpenGL application. */
class OpenGLApp
{
public:
    OpenGLApp(GLFWwindow* window, 
        std::string vertex_shader_file, 
        std::string fragment_shader_file);
    ~OpenGLApp();

    int Start();

    Camera& GetCamera();
    void SetRootObject(Object* root_object);
    // Main object for scene that all objects will be a child of
    Object* GetRootObject();

    void SetSkyColor(float r, float g, float b);
private:
    bool CreateShaderProgram(
        std::string vertex_file, 
        std::string fragment_file,
        GLuint* program_ptr);

    // Application/main loop
    bool can_start_;
    GLFWwindow* window_;
    GLuint shader_;
    float delta_;

    // Input
    bool w_down_;
    bool a_down_;
    bool s_down_;
    bool d_down_;
    bool q_down_;
    bool e_down_;
    bool in_window_;
    bool should_close_;
    float x_pos_;
    float y_pos_;
    float delta_x_;
    float delta_y_;
    float scroll_; 
    void ProcessInput();
    static void KeyCallback(GLFWwindow* window, int key, int scancode, 
        int action, int mods);
    static void CursorPositionCallback(GLFWwindow* window, 
        double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, 
        double xoffset, double yoffset);
    static void MouseButtonCallback(GLFWwindow* window, int button,
        int action, int mods);

    // Scene
    Camera camera_;
    Object* root_object_;
    // Sky color
    float sky_r_, sky_g_, sky_b_;
};

#endif // OPENGL_APP_H
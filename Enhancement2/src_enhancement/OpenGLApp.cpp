/* OpenGLApp.cpp
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#include "OpenGLApp.h"

OpenGLApp::OpenGLApp(GLFWwindow* window, 
    std::string vertex_shader_file, std::string fragment_shader_file)
{
    this->window_ = window;
    can_start_ = true;

    // Allow callbacks to have access to data from main loop
    glfwSetWindowUserPointer(window, this);

    // Register input callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    if (!CreateShaderProgram(vertex_shader_file, fragment_shader_file, 
        &shader_))
    {
        can_start_ = false;
    }

    delta_ = 0.0f;
    w_down_ = false;
    a_down_ = false;
    s_down_ = false;
    d_down_ = false;
    q_down_ = false;
    e_down_ = false;
    in_window_ = false;
    should_close_ = false;
    x_pos_ = 0.0f;
    y_pos_ = 0.0f;
    delta_x_ = 0.0f;
    delta_y_ = 0.0f;
    scroll_ = 10.0f;
    root_object_ = nullptr;

    SetSkyColor(0.0f, 0.0f, 0.0f);
}

OpenGLApp::~OpenGLApp()
{
    glDeleteShader(shader_);
    delete root_object_;
}

int OpenGLApp::Start()
{
    if (!can_start_)
    {
        return -1;
    }


    // Set the width and height of the OpenGL context
    int width;
    int height;
    glfwGetWindowSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    float last_time = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window_))
    {
        // Compute difference in time between frames
        float now = glfwGetTime();
        delta_ = now - last_time;
        last_time = now;

        // Update app based on polled input
        ProcessInput();

        // Enable depth testing so triangles are drawn correctly in 3D
        glEnable(GL_DEPTH_TEST);

        // Clear the screen to specified color and clear depth
        glClearColor(sky_r_, sky_g_, sky_b_, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shaders that were compiled
        glUseProgram(shader_);

        /* Create model view projection matrices to transform the model, move
         * the camera, and apply perspective */
        float time = (float) glfwGetTime();
        glm::mat4 view = camera_.GetViewMatrix();

        float aspect = (float) width / height;
        float ortho_y = 10.0f / scroll_, ortho_x = ortho_y * aspect;

        // Use perspective projection with 90 degree field of view.
        glm::mat4 projection =
            glm::perspective(45.0f, aspect, 3.0f, 1000.0f);

        // Get matrix uniform locations to supply matrices to shader program
        GLuint view_loc = glGetUniformLocation(shader_, "view");
        GLuint proj_loc = glGetUniformLocation(shader_, "projection");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));

        if (root_object_ != nullptr)
            root_object_->Draw();

        /* Show rendered trianglesand poll for events like closing and
         * moving the window */
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

Camera& OpenGLApp::GetCamera()
{
    return camera_;
}

void OpenGLApp::SetRootObject(Object* root_object)
{
    this->root_object_ = root_object;
}

Object* OpenGLApp::GetRootObject()
{
    return root_object_;
}

void OpenGLApp::SetSkyColor(float r, float g, float b)
{
    sky_r_ = r;
    sky_g_ = g;
    sky_b_ = b;
}

bool OpenGLApp::CreateShaderProgram(
    std::string vertex_file, 
    std::string fragment_file,
    GLuint* program_ptr)
{
    // Load shader files
    std::ifstream ifs;
    std::ostringstream oss;
    std::string line;

    // Load vertex shader file
    ifs.open(vertex_file);
    if (ifs.fail())
    {
        std::cout << "Could not read vertex shader '" << vertex_file <<
            "'." << std::endl;
        return false;
    }
    while (getline(ifs, line))
        oss << line << '\n';
    std::string vertex = oss.str();
    ifs.close();
    oss.str("");

    // Load fragment shader file
    ifs.open(fragment_file);
    if (ifs.fail())
    {
        std::cout << "Could not read fragment shader '" << fragment_file << 
            "'." << std::endl;
        return false;
    }
    while (getline(ifs, line))
        oss << line << '\n';
    std::string fragment = oss.str();
    ifs.close();
    oss.str("");

    // Variables to check for compiler and linker errors
    int success = 0;
    const int kInfoLogLength = 512;
    char info_log[kInfoLogLength];

    // Create program object
    GLuint program = glCreateProgram();

    // Create shader objects
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Supply shader source code to OpenGL
    const char* vertex_source = vertex.c_str();
    glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
    const char* fragment_source = fragment.c_str();
    glShaderSource(fragment_shader, 1, &fragment_source, nullptr);

    // Compile shaders and check for errors
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glGetShaderInfoLog(vertex_shader, kInfoLogLength, nullptr, info_log);
        std::cout << "Vertex shader failed to compile:" << std::endl <<
            info_log << std::endl;
        return false;
    }
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glGetShaderInfoLog(fragment_shader, kInfoLogLength, nullptr, info_log);
        std::cout << "Fragment shader failed to compile:" << std::endl <<
            info_log << std::endl;
        return false;
    }

    // Attach shaders to program
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    // Flag for shaders to be deleted when program is no longer used
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Link compiled code into single program
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glDeleteProgram(program);
        glGetProgramInfoLog(program, kInfoLogLength, nullptr, info_log);
        std::cout << "Program failed to link:" << std::endl << 
            info_log << std::endl;
        return false;
    }

    *program_ptr = program;
    return true;
}

void OpenGLApp::ProcessInput()
{
    if (should_close_)
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    if (in_window_)
    {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Change camera rotation based on mouse movement with speed of 0.2
        camera_.RotateYaw(-delta_x_ * 0.003f);
        camera_.RotatePitch(-delta_y_ * 0.003f);

        float move_speed = scroll_;

        // Move camera in different directions with speed based on mouse scroll
        if (w_down_)
            camera_.MoveForward(delta_ * move_speed);
        if (s_down_)
            camera_.MoveForward(-delta_ * move_speed);
        if (d_down_)
            camera_.MoveRight(delta_ * move_speed);
        if (a_down_)
            camera_.MoveRight(-delta_ * move_speed);
        if (e_down_)
            camera_.MoveUp(delta_ * move_speed);
        if (q_down_)
            camera_.MoveUp(-delta_ * move_speed);

        // Reset mouse position delta for next scrollCallback
        delta_x_ = 0.0f;
        delta_y_ = 0.0f;
    }
    else
    {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void OpenGLApp::KeyCallback(GLFWwindow* window, int key, int scancode, 
    int action, int mods)
{
    OpenGLApp* app = static_cast<OpenGLApp*>(glfwGetWindowUserPointer(window));

    // Set keyDown boolean variables when keys are pressed and released
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W)
            app->w_down_ = true;
        if (key == GLFW_KEY_A)
            app->a_down_ = true;
        if (key == GLFW_KEY_S)
            app->s_down_ = true;
        if (key == GLFW_KEY_D)
            app->d_down_ = true;
        if (key == GLFW_KEY_Q)
            app->q_down_ = true;
        if (key == GLFW_KEY_E)
            app->e_down_ = true;
        if (key == GLFW_KEY_ESCAPE)
            app->should_close_ = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_W)
            app->w_down_ = false;
        if (key == GLFW_KEY_A)
            app->a_down_ = false;
        if (key == GLFW_KEY_S)
            app->s_down_ = false;
        if (key == GLFW_KEY_D)
            app->d_down_ = false;
        if (key == GLFW_KEY_Q)
            app->q_down_ = false;
        if (key == GLFW_KEY_E)
            app->e_down_ = false;
    }
}

void OpenGLApp::CursorPositionCallback(GLFWwindow* window, 
    double xpos, double ypos)
{
    OpenGLApp* app = static_cast<OpenGLApp*>(glfwGetWindowUserPointer(window));

    /* Set mouse deltaXand deltaY to change in position 
     * from last mouse position */
    if (app->in_window_)
    {
        app->delta_x_ = xpos - app->x_pos_;
        app->delta_y_ = ypos - app->y_pos_;
    }
    app->x_pos_ = xpos;
    app->y_pos_ = ypos;
}

void OpenGLApp::ScrollCallback(GLFWwindow* window, 
    double xoffset, double yoffset)
{
    OpenGLApp* app = static_cast<OpenGLApp*>(glfwGetWindowUserPointer(window));

    // Increase the scroll (camera movement speed) by mouse scroll change
    app->scroll_ *= glm::pow(1.2f, yoffset);

    // Prevent camera speed from going negative or too fast
    if (app->scroll_ < 0.1f)
        app->scroll_ = 0.1f;
}

void OpenGLApp::MouseButtonCallback(GLFWwindow* window, int button, 
    int action, int mods)
{
    OpenGLApp* app = static_cast<OpenGLApp*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
    {
        app->in_window_ = !app->in_window_;
    }
}
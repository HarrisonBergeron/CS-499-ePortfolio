#include "OpenGLApp.h"

OpenGLApp::OpenGLApp(GLFWwindow* window, string vertexShader, string fragmentShader)
{
	this->window = window;
	canStart = true;
	// No limit to mouse movement, prevent mouse from going off screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Prevent sudden change in camera yaw/pitch after first mouse move event (large deltaX and deltaY)
	glfwSetCursorPos(window, 0, 0);

	// Allow callbacks to have access to data from main loop
	glfwSetWindowUserPointer(window, this);

	// Register input callback functions
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetScrollCallback(window, scrollCallback);
	if (!createShaderProgram(vertexShader, fragmentShader, &shader))
	{
		canStart = false;
	}

	delta = 0.0f;
	wDown = false;
	aDown = false;
	sDown = false;
	dDown = false;
	qDown = false;
	eDown = false;
	usePerspective = true;
	shouldClose = false;
	xPos = 0.0f;
	yPos = 0.0f;
	deltaX = 0.0f;
	deltaY = 0.0f;
	scroll = 10.0f;
}

OpenGLApp::~OpenGLApp()
{
	glDeleteShader(shader);
}

int OpenGLApp::start()
{
	if (!canStart)
	{
		glfwTerminate();
		return -1;
	}


	// Set the width and height of the OpenGL context
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);

	float lastTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// Compute difference in time between frames
		float now = glfwGetTime();
		delta = now - lastTime;
		processInput();
		lastTime = now;

		// Update app based on polled input
		processInput();

		// Enable depth testing so triangles are drawn correctly in 3D
		glEnable(GL_DEPTH_TEST);

		// Clear the screen to black and clear the depth buffer
		glClearColor(37 / 100.0f, 61 / 100.0f, 86 / 100.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shaders that were compiled
		glUseProgram(shader);

		// Create model view projection matrices to transform the model, move the camera, and apply perspective
		float time = (float) glfwGetTime();
		glm::mat4 view = camera.getViewMatrix();

		float aspect = (float) width / height;
		float orthoY = 10.0f, orthoX = orthoY * aspect;

		// Use perspective projection unless user presses P to change usePerspective to false
		glm::mat4 projection = 
			usePerspective
			? glm::perspective(45.0f, aspect, 0.1f, 1000.0f)
			: glm::ortho(-orthoX, orthoX, -orthoY, orthoY, 0.1f, 1000.0f);

		// Get matrix uniform locations to supply matrices to shader program
		GLuint viewLoc = glGetUniformLocation(shader, "view");
		GLuint projLoc = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		rootObject.draw();

		// Show rendered triangles and poll for events like closing and moving the window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

Camera& OpenGLApp::getCamera()
{
	return camera;
}

Object* OpenGLApp::getRootObject()
{
	return &rootObject;
}

bool OpenGLApp::createShaderProgram(string vertex, string fragment, GLuint* programPtr)
{
	// Variables to check for compiler and linker errors
	int success = 0;
	char infoLog[512];

	// Create program object
	GLuint program = glCreateProgram();

	// Create shader objects
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Supply shader source code to OpenGL
	const char* vertexSource = vertex.c_str();
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	const char* fragmentSource = fragment.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

	// Compile shaders and check for errors
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		cout << "Vertex shader failed to compile:" << endl << infoLog << endl;
		return false;
	}
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		cout << "Fragment shader failed to compile:" << endl << infoLog << endl;
		return false;
	}

	// Attach shaders to program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link compiled code into single program
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		cout << "Program failed to link:" << endl << infoLog << endl;
		return false;
	}

	*programPtr = program;
	return true;
}

void OpenGLApp::processInput()
{
	// Change camera rotation based on mouse movement with speed of 0.2
	camera.rotateYaw(-delta * deltaX * 0.2f);
	camera.rotatePitch(-delta * deltaY * 0.2f);

	float moveSpeed = scroll;

	// Move camera in different directions with speed based on mouse scroll
	if (wDown)
		camera.moveForward(delta * moveSpeed);
	if (sDown)
		camera.moveForward(-delta * moveSpeed);
	if (dDown)
		camera.moveRight(delta * moveSpeed);
	if (aDown)
		camera.moveRight(-delta * moveSpeed);
	if (eDown)
		camera.moveUp(delta * moveSpeed);
	if (qDown)
		camera.moveUp(-delta * moveSpeed);
	if (shouldClose)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// Reset mouse position delta for next scrollCallback
	deltaX = 0.0f;
	deltaY = 0.0f;
}

void OpenGLApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	OpenGLApp* app = (OpenGLApp*) glfwGetWindowUserPointer(window);

	// Set keyDown boolean variables when keys are pressed and released
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_W)
			app->wDown = true;
		if (key == GLFW_KEY_A)
			app->aDown = true;
		if (key == GLFW_KEY_S)
			app->sDown = true;
		if (key == GLFW_KEY_D)
			app->dDown = true;
		if (key == GLFW_KEY_Q)
			app->qDown = true;
		if (key == GLFW_KEY_E)
			app->eDown = true;
		if (key == GLFW_KEY_P)
			app->usePerspective = !app->usePerspective;
		if (key == GLFW_KEY_ESCAPE)
			app->shouldClose = true;
	}
	else if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W)
			app->wDown = false;
		if (key == GLFW_KEY_A)
			app->aDown = false;
		if (key == GLFW_KEY_S)
			app->sDown = false;
		if (key == GLFW_KEY_D)
			app->dDown = false;
		if (key == GLFW_KEY_Q)
			app->qDown = false;
		if (key == GLFW_KEY_E)
			app->eDown = false;
	}
}

void OpenGLApp::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	OpenGLApp* app = (OpenGLApp*)glfwGetWindowUserPointer(window);

	// Set mouse deltaX and deltaY to change in position from last mouse position
	app->deltaX = xpos - app->xPos;
	app->deltaY = ypos - app->yPos;
	app->xPos = xpos;
	app->yPos = ypos;
}

void OpenGLApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	OpenGLApp* app = (OpenGLApp*)glfwGetWindowUserPointer(window);

	// Increase the scroll (camera movement speed) by mouse scroll change
	app->scroll *= glm::pow(1.2f, yoffset);

	// Prevent camera speed from going negative or too fast
	if (app->scroll < 0.1f)
		app->scroll = 0.1f;
	if (app->scroll > 200.0f)
		app->scroll = 200.0f;
}

Object::Object()
{
	model = nullptr;
	texture = nullptr;
	transform = glm::identity<glm::mat4>();
}

void Object::attachModel(Model* model)
{
	this->model = model;
}

Model* Object::getModel()
{
	return model;
}

void Object::detachModel()
{
	model = nullptr;
}

void Object::attachTexture(Texture* texture)
{
	this->texture = texture;
}

Texture* Object::getTexture()
{
	return texture;
}

void Object::detachTexture()
{
	texture = nullptr;
}

void Object::setTransform(glm::mat4 transform)
{
	this->transform = transform;
}

glm::mat4 Object::getTransform()
{
	return transform;
}

void Object::addChild(Object* child)
{
	children.push_back(child);
}

const vector<Object*>& Object::getChildren()
{
	return children;
}

void Object::removeChild(Object* child)
{
	children.erase(find(children.begin(), children.end(), child));
}

void Object::draw()
{
	// Draw root node, aka no parent transform
	draw(glm::identity<glm::mat4>());
}

void Object::draw(glm::mat4 parentTransform)
{
	glm::mat4 combined = parentTransform * transform;
	// Draw this model with this transformed and inherited transform using texture
	if (model != nullptr)
	{
		// Get OpenGL shader currently in use so that the useTexture uniform can be supplied
		GLint shader = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader);
		if (texture != nullptr)
		{
			glUniform1i(glGetUniformLocation(shader, "useTexture"), true);
			texture->use("uTexture");
			model->draw(combined);
			texture->unuse();
		}
		else
		{
			glUniform1i(glGetUniformLocation(shader, "useTexture"), false);
			model->draw(combined);
		}
	}
	// Repeat process recursively for all children
	for (auto it = children.begin(); it != children.end(); it++)
	{
		if (*it != nullptr)
		{
			(*it)->draw(combined);
		}
	}
}

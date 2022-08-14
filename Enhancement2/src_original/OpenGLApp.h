#pragma once

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
using namespace std;

#include "Model.h"
#include "Camera.h"
#include "Texture.h"

// Objects can be grouped in hierarchies to combine transforms
class Object
{
public:
	Object();
	void attachModel(Model* model); // Attach model to be transformed and drawn by this object
	Model* getModel();
	void detachModel();
	void attachTexture(Texture* texture);
	Texture* getTexture();
	void detachTexture();
	void setTransform(glm::mat4 transform);
	glm::mat4 getTransform();
	void addChild(Object* child);
	const vector<Object*>& getChildren();
	void removeChild(Object* child);
	void draw(); // Used to draw this object without parent transforms (root node)
private:
	void draw(glm::mat4 parentTransform); // Calls draw of child objects recursively

	Model* model; // Model that will be transformed by 'glm::mat4 transform' and object's parent transforms
	Texture* texture;
	glm::mat4 transform;
	vector<Object*> children; // Children objects that will inherit transform of this object
};

class OpenGLApp
{
public:
	OpenGLApp(GLFWwindow* window, string vertexShader, string fragmentShader);
	~OpenGLApp();

	int start();

	Camera& getCamera();
	Object* getRootObject(); // Main object for scene that all objects will be a child of
private:
	bool createShaderProgram(string vertex, string fragment, GLuint* programPtr);

	// Application/main loop
	bool canStart;
	GLFWwindow* window;
	GLuint shader;
	float delta;

	// Input
	bool wDown;
	bool aDown;
	bool sDown;
	bool dDown;
	bool qDown;
	bool eDown;
	bool usePerspective;
	bool shouldClose;
	float xPos;
	float yPos;
	float deltaX;
	float deltaY;
	float scroll; 
	void processInput();
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	// Scene
	Camera camera;
	Object rootObject;
};


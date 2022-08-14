#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "OpenGLApp.h"

void init();
GLFWwindow* createWindow(int width, int height, string title);

int main(int argc, char* argv[])
{
	init();

	const int WIDTH = 800, HEIGHT = 600;
	// Create window and quit if failed
	GLFWwindow* window = createWindow(WIDTH, HEIGHT, "3D Scene: Textured Pyramid");
	if (window == nullptr)
	{
		cout << "Could not create window" << endl;
		glfwTerminate();
		return -1;
	}

	// Hardcoded vertex and fragment shader source code
	string vertexShader =
		"#version 440 core\n" \
		"layout(location = 0) in vec3 aPos;" \
		"layout(location = 1) in vec4 color;" \
		"layout(location = 2) in vec2 tex;" \
		"layout(location = 3) in vec3 normal;" \
		"out vec4 vertexColor;" \
		"out vec2 vertexTex;" \
		"out vec3 vertexNormal;" \
		"out vec3 vertexFragPos;" \

		"uniform mat4 model;" \
		"uniform mat4 view;" \
		"uniform mat4 projection;" \

		"void main()" \
		"{" \
		"gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);" \
		"vertexColor = color;" \
		"vertexTex = tex;" \

		"vertexNormal = mat3(transpose(inverse(model))) * normal;" \

		"vertexFragPos = vec3(model * vec4(aPos, 1.0f));" \
		"}";
	string fragmentShader =
		"#version 440 core\n" \
		"in vec4 vertexColor;" \
		"in vec2 vertexTex;" \
		"in vec3 vertexNormal;" \
		"in vec3 vertexFragPos;" \
		"out vec4 FragColor;" \
		"uniform sampler2D uTexture;" \

		"void main()" \
		"{" \
		"vec3 lightColor = vec3(1.5, 1.5, 1.0);" \
		"vec3 lightDir = vec3(2.0, -1.5, -1.0);" \

		"float impact = max(dot(normalize(vertexNormal), normalize(-lightDir)), 0.0);" \
		"vec3 diffuse = impact * lightColor;"

		"float ambientStrength = 0.4;" \
		"vec3 ambient = ambientStrength * vec3(0.8, 0.8, 1.0);" \

		"vec3 phong = ambient + diffuse;" \

		"FragColor = vec4(phong, 1.0) * vertexColor * texture(uTexture, vertexTex);" \
		"}";

	// Create application with specified shaders
	OpenGLApp app(window, vertexShader, fragmentShader);

	// Load resources
	Texture leaves;
	leaves.load("leaves.jpg");
	Texture bark;
	bark.load("bark.jpg");
	Texture grass;
	grass.load("grass.jpg");
	Texture roadTexture;
	roadTexture.load("road.png");
	Texture waterTexture;
	waterTexture.load("water.jpg");
	Texture buildingTexture;
	buildingTexture.load("building.png");
	Texture building2Texture;
	building2Texture.load("building2.png");
	Texture carColor;
	carColor.load("carcolor.png");
	Texture tireColor;
	tireColor.load("tirecolor.png");

	// Create scene
	Model cylinder = Model::createCylinder(14, 14, 1.0f, 5.0f);
	Model sphere = Model::createSphere(50, 50, 10.0f, 10.0f);
	Model plane = Model::createXZPlane(40, 40, 15.0f, 15.0f);
	Model plane2 = Model::createXZPlane(40, 40, 1.0f, 1.0f);
	Model cube = Model::createCube();

	Object ground;
	ground.attachTexture(&grass);
	ground.attachModel(&plane);
	ground.setTransform(glm::scale(glm::vec3(150.0f, 150.0f, 150.0f)) * glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)));
	app.getRootObject()->addChild(&ground);

	Object treeTrunk;
	treeTrunk.attachTexture(&bark);
	treeTrunk.attachModel(&cylinder);
	treeTrunk.setTransform(glm::scale(glm::vec3(0.5f, 10.0f, 0.5f)) * glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));

	Object treeTop;
	treeTop.attachTexture(&leaves);
	treeTop.attachModel(&sphere);
	treeTop.setTransform(glm::translate(glm::vec3(0.0f, 20.0f, 0.0f)) * glm::scale(glm::vec3(2.0, 2.7f, 2.0f)));

	Object tree;
	tree.setTransform(glm::translate(glm::vec3(20.0f, 0.0f, -40.0f)) * glm::scale(glm::vec3(1.0f, 1.4f, 1.0f)));
	tree.addChild(&treeTrunk);
	tree.addChild(&treeTop);
	app.getRootObject()->addChild(&tree);

	Object tree2;
	tree2.setTransform(glm::translate(glm::vec3(30.0f, 0.0f, -50.0f)) * glm::scale(glm::vec3(1.0f, 1.1f, 1.0f)));
	tree2.addChild(&treeTrunk);
	tree2.addChild(&treeTop);

	app.getRootObject()->addChild(&tree2);

	Object tree3;
	tree3.setTransform(glm::translate(glm::vec3(20.0f, 0.0f, -70.0f))* glm::scale(glm::vec3(1.0f, 1.3f, 1.0f)));
	tree3.addChild(&treeTrunk);
	tree3.addChild(&treeTop);

	app.getRootObject()->addChild(&tree3);

	Object tree4;
	tree4.setTransform(glm::translate(glm::vec3(30.0f, 0.0f, -90.0f))* glm::scale(glm::vec3(1.0f, 1.1f, 1.0f)));
	tree4.addChild(&treeTrunk);
	tree4.addChild(&treeTop);

	app.getRootObject()->addChild(&tree4);

	Object bigBush;
	bigBush.attachTexture(&leaves);
	bigBush.attachModel(&sphere);
	bigBush.setTransform(glm::translate(glm::vec3(20.0f, 5.0f, -20.0f))* glm::scale(glm::vec3(7.0, 7.0f, 7.0f)));

	app.getRootObject()->addChild(&bigBush);

	Object bigBush2;
	bigBush2.attachTexture(&leaves);
	bigBush2.attachModel(&sphere);
	bigBush2.setTransform(glm::translate(glm::vec3(-20.0f, 3.0f, -20.0f))* glm::scale(glm::vec3(4.0, 4.0f, 4.0f)));

	app.getRootObject()->addChild(&bigBush2);

	Object road;
	road.setTransform(glm::translate(glm::vec3(0.0f, 0.03f, 0.0f)) * glm::scale(glm::vec3(15.0f, 100.0f, 150.0f)) * glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)) * glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f)));
	road.attachTexture(&roadTexture);
	road.attachModel(&plane2);

	app.getRootObject()->addChild(&road);

	Object water;
	water.setTransform(glm::translate(glm::vec3(-130.0f, 0.03f, 0.0f))* glm::scale(glm::vec3(100.0f, 100.0f, 80.0f))* glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)));
	water.attachTexture(&waterTexture);
	water.attachModel(&plane);

	app.getRootObject()->addChild(&water);

	Object building;
	building.setTransform(glm::translate(glm::vec3(-50.0f, 0.0f, -200.0f)) * glm::scale(glm::vec3(7.0f, 15.0f, 7.0f)) * glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
	building.attachTexture(&buildingTexture);
	building.attachModel(&cube);
	
	app.getRootObject()->addChild(&building);

	Object building2;
	building2.setTransform(glm::translate(glm::vec3(-70.0f, 0.0f, -200.0f))* glm::scale(glm::vec3(7.0f, 7.0f, 7.0f))* glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
	building2.attachTexture(&building2Texture);
	building2.attachModel(&cube);

	app.getRootObject()->addChild(&building2);

	Object building3;
	building3.setTransform(glm::translate(glm::vec3(-90.0f, 0.0f, -200.0f))* glm::scale(glm::vec3(7.0f, 4.0f, 7.0f))* glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
	building3.attachTexture(&buildingTexture);
	building3.attachModel(&cube);

	app.getRootObject()->addChild(&building3);

	Object building4;
	building4.setTransform(glm::translate(glm::vec3(-70.0f, 0.0f, -230.0f))* glm::scale(glm::vec3(7.0f, 12.0f, 7.0f))* glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
	building4.attachTexture(&buildingTexture);
	building4.attachModel(&cube);

	app.getRootObject()->addChild(&building4);

	Object building5;
	building5.setTransform(glm::translate(glm::vec3(-30.0f, 0.0f, -230.0f))* glm::scale(glm::vec3(7.0f, 12.0f, 7.0f))* glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
	building5.attachTexture(&building2Texture);
	building5.attachModel(&cube);

	app.getRootObject()->addChild(&building5);

	Object car;
	car.setTransform(glm::translate(glm::vec3(7.0f, 2.0f, -15.0f)) * glm::scale(glm::vec3(0.9f, 0.9f, 0.9f))* glm::scale(glm::vec3(1.2f, 0.9f, 1.0f)));

	Object carBody;
	carBody.setTransform(glm::scale(glm::vec3(2.0f, 1.0f, 5.0f)));
	carBody.attachModel(&cube);
	carBody.attachTexture(&carColor);

	Object carTop;
	carTop.setTransform(glm::translate(glm::vec3(0.0f, 1.75f, 0.0f)) * glm::scale(glm::vec3(2.0f, 0.75f, 3.0f)));
	carTop.attachModel(&cube);
	carTop.attachTexture(&carColor);

	Object carTire1;
	carTire1.setTransform(glm::translate(glm::vec3(-1.52f, -1.0f, -3.5f))* glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)) * glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
	carTire1.attachModel(&cylinder);
	carTire1.attachTexture(&tireColor);

	Object carTire2;
	carTire2.setTransform(glm::translate(glm::vec3(1.52f, -1.0f, -3.5f))* glm::scale(glm::vec3(0.5f, 1.0f, 1.0f))* glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
	carTire2.attachModel(&cylinder);
	carTire2.attachTexture(&tireColor);

	Object carTire3;
	carTire3.setTransform(glm::translate(glm::vec3(-1.52f, -1.0f, 3.5f))* glm::scale(glm::vec3(0.5f, 1.0f, 1.0f))* glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
	carTire3.attachModel(&cylinder);
	carTire3.attachTexture(&tireColor);

	Object carTire4;
	carTire4.setTransform(glm::translate(glm::vec3(1.52f, -1.0f, 3.5f))* glm::scale(glm::vec3(0.5f, 1.0f, 1.0f))* glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
	carTire4.attachModel(&cylinder);
	carTire4.attachTexture(&tireColor);

	car.addChild(&carBody);
	car.addChild(&carTop);
	car.addChild(&carTire1);
	car.addChild(&carTire2);
	car.addChild(&carTire3);
	car.addChild(&carTire4);

	app.getRootObject()->addChild(&car);

	Object backgroundBushes[20];
	for (int i = 0; i < 20; i++)
	{
		backgroundBushes[i].setTransform(glm::translate(glm::vec3(-100.0f + i * 10.0f, 0.0f, -300.0f)) * glm::scale(glm::vec3(5.0f, 5.0f, 5.0f)));
		backgroundBushes[i].attachTexture(&leaves);
		backgroundBushes[i].attachModel(&sphere);

		app.getRootObject()->addChild(&backgroundBushes[i]);
	}



	app.getCamera().setPos(glm::vec3(0.0f, 6.0f, 10.0f));

	// Start main loop
	app.start();

	// Correctly quit glfw
	glfwTerminate();
	return 0;
}

void init() 
{
	// Seed random numbers
	srand(time(nullptr));
	
	// Initialize glfw for OpenGL with modern 4.4 version
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	stbi_set_flip_vertically_on_load(true);
}

GLFWwindow* createWindow(int width, int height, string title)
{
	// Create window and OpenGL context
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr)
		return nullptr;
	glfwMakeContextCurrent(window);

	// Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return nullptr;
	}
	return window;
}


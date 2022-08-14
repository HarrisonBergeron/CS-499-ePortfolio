/* main.cpp
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "OpenGLApp.h"

void Init();
GLFWwindow* CreateWindow(int width, int height, std::string title);

int main(int argc, char* argv[])
{
    Init();

    const int kWidth = 1000, kHeight = 800;
    // Create window and quit if failed
    GLFWwindow* window = CreateWindow(kWidth, kHeight, 
        "3D Scene");
    if (window == nullptr)
    {
        std::cout << "Could not create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create application with specified shaders
    OpenGLApp app(window, "shader.vert", "shader.frag");

    // Load resources
    Texture leaves;
    leaves.Load("leaves.jpg");
    Texture bark;
    bark.Load("bark.jpg");
    Texture grass;
    grass.Load("grass.jpg");
    Texture road_texture;
    road_texture.Load("road.png");
    Texture water_texture;
    water_texture.Load("water.jpg");
    Texture building_texture;
    building_texture.Load("building.png");
    Texture building2_texture;
    building2_texture.Load("building2.png");
    Texture car_color_texture;
    car_color_texture.Load("carcolor.png");
    Texture tire_color_texture;
    tire_color_texture.Load("tirecolor.png");

    // Create scene
    Model cylinder = Model::CreateCylinder(14, 14, 1.0f, 5.0f);
    Model sphere = Model::CreateSphere(50, 50, 10.0f, 10.0f);
    Model plane = Model::CreateXZPlane(40, 40, 15.0f, 15.0f);
    Model plane2 = Model::CreateXZPlane(40, 40, 1.0f, 1.0f);
    Model cube = Model::CreateCube();

    Object* scene = new Object();

    Object* ground = new Object();
    ground->AttachTexture(&grass);
    ground->AttachModel(&plane);
    ground->SetTransform(glm::scale(glm::vec3(150.0f, 150.0f, 150.0f)) * 
        glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)));
    scene->AddChild(ground);

    Object* tree_trunk = new Object();
    tree_trunk->AttachTexture(&bark);
    tree_trunk->AttachModel(&cylinder);
    tree_trunk->SetTransform(glm::scale(glm::vec3(0.5f, 10.0f, 0.5f)) * 
        glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));

    Object* tree_top = new Object();
    tree_top->AttachTexture(&leaves);
    tree_top->AttachModel(&sphere);
    tree_top->SetTransform(glm::translate(glm::vec3(0.0f, 20.0f, 0.0f)) * 
        glm::scale(glm::vec3(2.0, 2.7f, 2.0f)));

    Object* tree = new Object();
    tree->SetTransform(glm::translate(glm::vec3(20.0f, 0.0f, -40.0f)) * 
        glm::scale(glm::vec3(1.0f, 1.4f, 1.0f)));
    tree->AddChild(tree_trunk);
    tree->AddChild(tree_top);

    scene->AddChild(tree);

    Object* tree2 = new Object();
    tree2->SetTransform(glm::translate(glm::vec3(30.0f, 0.0f, -50.0f)) * 
        glm::scale(glm::vec3(1.0f, 1.1f, 1.0f)));
    tree2->AddChild(new Object(tree_trunk));
    tree2->AddChild(new Object(tree_top));

    scene->AddChild(tree2);

    Object* tree3 = new Object();
    tree3->SetTransform(glm::translate(glm::vec3(20.0f, 0.0f, -70.0f)) * 
        glm::scale(glm::vec3(1.0f, 1.3f, 1.0f)));
    tree3->AddChild(new Object(tree_trunk));
    tree3->AddChild(new Object(tree_top));

    scene->AddChild(tree3);

    Object* tree4 = new Object();
    tree4->SetTransform(glm::translate(glm::vec3(30.0f, 0.0f, -90.0f)) * 
        glm::scale(glm::vec3(1.0f, 1.1f, 1.0f)));
    tree4->AddChild(new Object(tree_trunk));
    tree4->AddChild(new Object(tree_top));

    scene->AddChild(tree4);

    Object* big_bush = new Object();
    big_bush->AttachTexture(&leaves);
    big_bush->AttachModel(&sphere);
    big_bush->SetTransform(glm::translate(glm::vec3(20.0f, 5.0f, -20.0f)) * 
        glm::scale(glm::vec3(7.0, 7.0f, 7.0f)));

    scene->AddChild(big_bush);

    Object* big_bush2 = new Object();
    big_bush2->AttachTexture(&leaves);
    big_bush2->AttachModel(&sphere);
    big_bush2->SetTransform(glm::translate(glm::vec3(-20.0f, 3.0f, -20.0f)) * 
        glm::scale(glm::vec3(4.0, 4.0f, 4.0f)));

    scene->AddChild(big_bush2);

    Object* road = new Object();
    road->SetTransform(glm::translate(glm::vec3(0.0f, 0.03f, 0.0f)) * 
        glm::scale(glm::vec3(15.0f, 100.0f, 150.0f)) * 
        glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)) * 
        glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f)));
    road->AttachTexture(&road_texture);
    road->AttachModel(&plane2);

    scene->AddChild(road);

    Object* water = new Object();
    water->SetTransform(glm::translate(glm::vec3(-130.0f, 0.03f, 0.0f)) * 
        glm::scale(glm::vec3(100.0f, 100.0f, 80.0f)) * 
        glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)));
    water->AttachTexture(&water_texture);
    water->AttachModel(&plane);

    scene->AddChild(water);

    Object* building = new Object();
    building->SetTransform(glm::translate(glm::vec3(-50.0f, 0.0f, -200.0f)) * 
        glm::scale(glm::vec3(7.0f, 15.0f, 7.0f)) * 
        glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    building->AttachTexture(&building_texture);
    building->AttachModel(&cube);
    
    scene->AddChild(building);

    Object* building2 = new Object();
    building2->SetTransform(glm::translate(glm::vec3(-70.0f, 0.0f, -200.0f)) * 
        glm::scale(glm::vec3(7.0f, 7.0f, 7.0f)) * 
        glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    building2->AttachTexture(&building2_texture);
    building2->AttachModel(&cube);

    scene->AddChild(building2);

    Object* building3 = new Object();
    building3->SetTransform(glm::translate(glm::vec3(-90.0f, 0.0f, -200.0f)) * 
        glm::scale(glm::vec3(7.0f, 4.0f, 7.0f)) * 
        glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    building3->AttachTexture(&building_texture);
    building3->AttachModel(&cube);

    scene->AddChild(building3);

    Object* building4 = new Object();
    building4->SetTransform(glm::translate(glm::vec3(-70.0f, 0.0f, -230.0f)) * 
        glm::scale(glm::vec3(7.0f, 12.0f, 7.0f)) * 
        glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    building4->AttachTexture(&building_texture);
    building4->AttachModel(&cube);

    scene->AddChild(building4);

    Object* building5 = new Object();
    building5->SetTransform(glm::translate(glm::vec3(-30.0f, 0.0f, -230.0f)) * 
        glm::scale(glm::vec3(7.0f, 12.0f, 7.0f)) * 
        glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    building5->AttachTexture(&building2_texture);
    building5->AttachModel(&cube);

    scene->AddChild(building5);

    Object* car = new Object();
    car->SetTransform(glm::translate(glm::vec3(7.0f, 2.0f, -15.0f)) * 
        glm::scale(glm::vec3(0.9f, 0.9f, 0.9f)) * 
        glm::scale(glm::vec3(1.2f, 0.9f, 1.0f)));

    Object* car_body = new Object();
    car_body->SetTransform(glm::scale(glm::vec3(2.0f, 1.0f, 5.0f)));
    car_body->AttachModel(&cube);
    car_body->AttachTexture(&car_color_texture);

    Object* car_top = new Object();
    car_top->SetTransform(glm::translate(glm::vec3(0.0f, 1.75f, 0.0f)) * 
        glm::scale(glm::vec3(2.0f, 0.75f, 3.0f)));
    car_top->AttachModel(&cube);
    car_top->AttachTexture(&car_color_texture);

    Object* car_tire1 = new Object();
    car_tire1->SetTransform(glm::translate(glm::vec3(-1.52f, -1.0f, -3.5f)) * 
        glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)) * 
        glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
    car_tire1->AttachModel(&cylinder);
    car_tire1->AttachTexture(&tire_color_texture);

    Object* car_tire2 = new Object();
    car_tire2->SetTransform(glm::translate(glm::vec3(1.52f, -1.0f, -3.5f)) * 
        glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)) * 
        glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
    car_tire2->AttachModel(&cylinder);
    car_tire2->AttachTexture(&tire_color_texture);

    Object* car_tire3 = new Object();
    car_tire3->SetTransform(glm::translate(glm::vec3(-1.52f, -1.0f, 3.5f)) * 
        glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)) * 
        glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
    car_tire3->AttachModel(&cylinder);
    car_tire3->AttachTexture(&tire_color_texture);

    Object* car_tire4 = new Object();
    car_tire4->SetTransform(glm::translate(glm::vec3(1.52f, -1.0f, 3.5f)) * 
        glm::scale(glm::vec3(0.5f, 1.0f, 1.0f)) * 
        glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f)));
    car_tire4->AttachModel(&cylinder);
    car_tire4->AttachTexture(&tire_color_texture);

    car->AddChild(car_body);
    car->AddChild(car_top);
    car->AddChild(car_tire1);
    car->AddChild(car_tire2);
    car->AddChild(car_tire3);
    car->AddChild(car_tire4);

    scene->AddChild(car);

    const int kNumBushes = 20;
    Object* background_bushes[kNumBushes];
    for (int i = 0; i < kNumBushes; i++)
    {
        background_bushes[i] = new Object();
        background_bushes[i]->SetTransform(
            glm::translate(glm::vec3(-100.0f + i * 10.0f, 0.0f, -300.0f)) * 
            glm::scale(glm::vec3(5.0f, 5.0f, 5.0f)));
        background_bushes[i]->AttachTexture(&leaves);
        background_bushes[i]->AttachModel(&sphere);

        scene->AddChild(background_bushes[i]);
    }

    app.GetCamera().SetPos(glm::vec3(0.0f, 6.0f, 10.0f));
    // Light blue
    app.SetSkyColor(0.37f, 0.61f, 0.86f);






    // ************************************************************************
    /* 
     * ENHANCEMENT 1 DEMONSTRATION (graphical output)
     *
     * Demonstrates deep copy function by making a copy of the entire scene
     * graph but translated to the right by 400 units. The intention of this
     * function is to allow for copying an entire tree data structure by
     * traversing the tree and copying every descendant node. It uses a stack
     * data structure to traverse the tree. This function also shows that 
     * recursion is not required to emulate recursive behavior, and a stack 
     * can be used to achieve the same purpose. */

    /* Deep copy the node containing all scene objects (see Object.cpp for
     * DeepCopy() implementation). */
    Object* scene_deep_copy = scene->DeepCopy();
    // Move to the right to show there are now two scene copies
    scene_deep_copy->SetTransform(
        glm::translate(glm::vec3(400.0f, 0.0f, 0.0f)));
    // Root object will contain both scenes
    Object* root_object = new Object();
    root_object->AddChild(scene);
    root_object->AddChild(scene_deep_copy);
    app.SetRootObject(root_object);
    // ************************************************************************





    // ************************************************************************
    /* 
     * ENHANCEMENT 2 DEMONSTRATION (console output)
     *
     * Demonstrate radix sorting function by adding child nodes to a parent
     * with different indices and sorting the children by index to control the
     * order that children are drawn on the screen. This sorting function is 
     * normally not invoked manually but is invoked automatically before an 
     * Object is drawn. The intention of this sorting algorithm is to use a 
     * stable non-comparison-based sorting technique that has a time complexity
     * better than O(n log n). It is often desirable to be able to control the
     * order that OpenGL objects are drawn, and this function achieves that
     * purpose. */

    // Lambda that creates an object with a given index.
    auto object_with_index = [](unsigned int index)
    {
        Object* object = new Object();
        object->SetDrawIndex(index);
        return object;
    };
    // Create a list of objects with different draw indices for sorting.
    std::vector<Object*> objects_to_sort
    {
        object_with_index(77),
        object_with_index(358),
        object_with_index(4),
        object_with_index(144),
        object_with_index(13),
        object_with_index(65),
        object_with_index(9009),
        object_with_index(11),
        object_with_index(65),
        object_with_index(1),
        object_with_index(86),
        object_with_index(130)
    };

    // Create a parent Object to hold the objects to be sorted.
    Object* sort_parent = new Object();
    sort_parent->AddAllChildren(objects_to_sort);

    // Print Object draw indices before sorting.
    const std::vector<Object*>& children = sort_parent->GetChildren();
    std::cout << "Before sort: " << std::endl;
    for (auto it = children.begin(); it != children.end(); it++)
        std::cout << (*it)->GetDrawIndex() << " ";
    std::cout << std::endl;

    // Manually call sort function
    sort_parent->SortChildrenByDrawIndex();

    // Print Object draw indices after sorting
    std::cout << "After sort: " << std::endl;
    for (auto it = children.begin(); it != children.end(); it++)
        std::cout << (*it)->GetDrawIndex() << " ";
    std::cout << std::endl << std::endl;
    // ************************************************************************





    // ************************************************************************
    /* 
     * ENHANCEMENT 3 DEMONSTRATION (graphical and console output)
     *
     * To demonstrate the Collapse() function, a literal binary *tree* is
     * constructed using individual branches that are rotated and translated
     * at each level of the hierarchy. Lambdas are used to recursively
     * build the tree branches and place leaves on top. Two trees are then
     * generated: one represented by a tree data structure, and one represented
     * by a list that is a "collapsed" version of the tree structure. The 
     * collapsed tree loses hierarchy and individual transform information, but 
     * maintains the transform of each individual branch. The intention of this
     * function is to provide a method for converting a tree data structure
     * into a one dimensional list data structure. Converting between data 
     * structures with more than one dimension into one dimensional data
     * structures can be useful for simplifying, storing, or transmitting 
     * data. */

    /* make_branch is a lambda function to create on offshoot branch of a
     * parent branch with a set size and angle.
     * 
     * branch_angle: How much to rotate this branch in relation to parent.
     * branch_len: How long the branch is in 3D space. */
    std::function<Object*(float, float)> make_branch = 
        [&cylinder, &bark](float branch_angle, float branch_len)
    {
        /* Create a dummy object to hold the branch. This will prevent scale
         * transformations in the "branch" object from being inherited by
         * child branches. */ 
        Object* container = new Object();
        /* Move the branch up and rotate it to build tree with branches that
         * spread upward and outward. */
        container->SetTransform(
            glm::rotate(branch_angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::translate(glm::vec3(0.0f, branch_len, 0.0f)));
        /* Create actual branch object to hold cylinder model that is stretched
         * to the appropriate length. */
        Object* branch = new Object();
        branch->AttachModel(&cylinder);
        branch->AttachTexture(&bark);
        branch->SetTransform(
            glm::scale(glm::vec3(0.5f, 0.5f * branch_len, 0.5f)) *
            glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)));
        container->AddChild(branch);
        return container;
    };
    
    /* make_tree is a lambda function to create a 3D binary tree Object 
     * recursively by calling itself until a certain depth is reached.
     * 
     * depth: How many levels will this tree have?
     * angle: Starting angle for branch "spread". 
     * len: Starting length for branches. 
     * angle_mul: Scale factor for branch angle on next recursive call.
     * len_mul: Scale factor for branch length on next recursive call. */
    std::function<Object*(int, float, float, float, float)> make_tree;
    make_tree = 
        [&make_tree, &make_branch, 
         &leaves, &sphere]
        (int depth, float angle, float len, float angle_mul, float len_mul)
    {
        // Create a dummy object to hold each branch in a group.
        Object* branch = new Object();

        // Create the left and right branches at opposite angles.
        Object* branch_left = make_branch(angle, len);
        branch->AddChild(branch_left);
        Object* branch_right = make_branch(-angle, len);
        branch->AddChild(branch_right);

        /* Scale the angle and length to control how the tree grows
         * at the next level. */
        angle *= angle_mul;
        len *= len_mul;
        // Decrease depth so the tree doesn't grow infinitely large.
        depth--;
        // If not at the highest branch, add more branches recursively.
        if (depth > 0)
        {
            branch_left->AddChild(
                make_tree(depth, angle, len, angle_mul, len_mul));
            branch_right->AddChild(
                make_tree(depth, angle, len, angle_mul, len_mul));
        }
        // If at the highest branch, add leaves.
        else
        {
            // Create leaves.
            Object* branch_end = new Object();
            branch_end->AttachModel(&sphere);
            branch_end->AttachTexture(&leaves);

            /* Set the size of the leaves to be proportional to the size of the
             * connecting branch. */
            const float kEndSize = len * 0.5f;
            branch_end->SetTransform(
                glm::scale(glm::vec3(kEndSize, kEndSize, kEndSize)));
            
            // Add leaves to branches.
            branch_left->AddChild(branch_end);
            branch_right->AddChild(new Object(branch_end));
        }
        return branch;
    };

    // Generate the binary tree recursively with custom arguments.
    Object* generated_tree = make_tree(6, 0.76f, 20.0f, 0.85f, 0.65f);
    // Move the tree backward into the scene
    generated_tree->SetTransform(
        glm::translate(glm::vec3(0.0f, 0.0f, -60.0f)));
    scene->AddChild(generated_tree);

    /* "Collapse" the multi-level binary tree into one parent node with a
     * long list of children. Throws out hierarchical information but maintains
     * position and orientation of each object. */
    Object* collapsed_generated_tree = generated_tree->Collapse();
    // Move the collapsed tree behind the original tree.
    collapsed_generated_tree->SetTransform(
        glm::translate(glm::vec3(0.0f, 0.0f, -60.0f)));
    scene->AddChild(collapsed_generated_tree);

    /* The original tree node will have only two children: the two branches at 
     * the base of the binary tree. Further descendents are stored at higher 
     * levels within the tree. On the other hand, the collapsed version
     * will have only one level of all 379 Objects for children. Both objects 
     * appear identical in the scene despite having different data structures 
     * representing them. One is a tree and the other is essentially a list. 
     * Printing the number of children in each node shows this, but the two
     * trees in the rendered scene are graphically the same. */
    std::cout << "Number of children in tree node: " << 
        generated_tree->GetChildren().size() << std::endl;
    std::cout << "Number of objects in collapsed tree node: " <<
        collapsed_generated_tree->GetChildren().size() << std::endl;
    // ************************************************************************




    // Start main loop
    app.Start();

    // Correctly quit glfw
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void Init() 
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

GLFWwindow* CreateWindow(int width, int height, std::string title)
{
    // Create window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), 
        nullptr, nullptr);
    if (window == nullptr)
        return nullptr;
    glfwMakeContextCurrent(window);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        return nullptr;
    }
    return window;
}


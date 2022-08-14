/* Object.h
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stack>
#include <vector>
#include <functional>
#include <utility>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Camera.h"
#include "Texture.h"

/* The Object class represents a scene graph node. Objects can be parented to
 * other objects to create an object hierarchy tree where child objects
 * inherit the 3D transform of their parents. This class exists to make it
 * easier to construct and manipulate 3D scenes that can be composed in a
 * hierarchical fashion. */
class Object
{
public:
    Object();
    // Shallow copy constructor
    Object(const Object* other);
    // Attach model to be transformed and drawn by this object
    void AttachModel(Model* model);
    Model* GetModel();
    void DetachModel();
    void AttachTexture(Texture* texture);
    Texture* GetTexture();
    void DetachTexture();
    void SetTransform(glm::mat4 transform);
    glm::mat4 GetTransform();
    void AddChild(Object* child);
    void RemoveChild(Object* child);
    void AddAllChildren(const std::vector<Object*>& children);
    void RemoveAllChildren();
    const std::vector<Object*>& GetChildren();
    // Used to draw this object without parent transforms (root node)
    void Draw();

    // ************************************************************************
    // ENHANCEMENTS
    ~Object();
    /* DeepCopy() copies a node and all of its descendents while maintaining
     * the tree structure. This is a useful function to include in applications
     * with tree data structures because shallow copies do not reach deeper
     * levels of the hierarchy. This function uses a stack data structure
     * instead of recursion to demonstrate that stack data structures can
     * mimic recursive behavior perfectly. */
    Object* DeepCopy();
    /* Set the draw index of this object. Before being displayed, it will be
     * sorted with respect to its siblings so that it will be drawn before
     * or after siblings based on their draw index. This drawing order does
     * not apply to objects in higher or lower levels of the tree. Being able
     * to change the drawing order of objects in OpenGL is crucial to
     * controlling how a scene is rendered. For example, it is often the case
     * that transparent objects should be drawn last. This functions allows
     * this behavior. */
    void SetDrawIndex(unsigned int draw_index);
    unsigned int GetDrawIndex();
    /* Sorts the children of this object according to the indices supplied
     * by the SetDrawIndex() function above. This function uses a radix sort,
     * which is a stable sort that iteratively sorts a list of numbers digit
     * by digit starting with their least significant digit and ending with
     * their most. Each iteration of radix sort uses a counting sort, which
     * is also a stable sort that puts numbers in order based on only one
     * digit. The  */
    void SortChildrenByDrawIndex();
    /* This function collapses an object tree into a single object with one
     * list of child nodes. No matter how many levels the original tree had,
     * all descendents will be combined into a single list of nodes. In order
     * to preserve transformations, transform matrices will be combined with
     * parent transform matrices recursively. The original hierarchy and
     * individual transformation information will be destroyed. A function
     * like this is useful in applications where a complicated scene graph is
     * not desired and needs to be broken down. */
    Object* Collapse();
private:
    unsigned int draw_index_;
    // END ENHANCEMENTS
    // ************************************************************************

    // Calls draw of child objects recursively
    void Draw(glm::mat4 parent_transform);

   
    /* Model that will be transformed by 'glm::mat4 transform' and object's
     * parent transforms. */
    Model* model_;
    Texture* texture_;
    glm::mat4 transform_;
    // Children objects that will inherit transform of this object
    std::vector<Object*> children_;
};

#endif
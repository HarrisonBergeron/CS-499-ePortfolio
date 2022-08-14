/* Object.cpp
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#include "Object.h"

Object::Object()
    : draw_index_(0), model_(nullptr), texture_(nullptr), 
      transform_(glm::identity<glm::mat4>())
{}

Object::Object(const Object* other)
    : draw_index_(other->draw_index_), model_(other->model_), 
      texture_(other->texture_), transform_(other->transform_) 
{}

void Object::AttachModel(Model* model)
{
    this->model_ = model;
}

Model* Object::GetModel()
{
    return model_;
}

void Object::DetachModel()
{
    model_ = nullptr;
}

void Object::AttachTexture(Texture* texture)
{
    this->texture_ = texture;
}

Texture* Object::GetTexture()
{
    return texture_;
}

void Object::DetachTexture()
{
    texture_ = nullptr;
}

void Object::SetTransform(glm::mat4 transform)
{
    this->transform_ = transform;
}

glm::mat4 Object::GetTransform()
{
    return transform_;
}

void Object::AddChild(Object* child)
{
    children_.push_back(child);
}

void Object::RemoveChild(Object* child)
{
    children_.erase(find(children_.begin(), children_.end(), child));
}

void Object::AddAllChildren(const std::vector<Object*>& children)
{
    for (auto it = children.begin(); it != children.end(); it++)
    {
        this->children_.push_back(*it);
    }
}

void Object::RemoveAllChildren()
{
    children_.clear();
}

const std::vector<Object*>& Object::GetChildren()
{
    return children_;
}

void Object::Draw()
{
    // Draw root node, aka no parent transform
    Draw(glm::identity<glm::mat4>());
}

Object::~Object()
{
    for (auto it = children_.begin(); it != children_.end(); it++)
    {
        delete *it;
    }
}

//*****************************************************************************
// ENHANCEMENTS

void Object::SetDrawIndex(unsigned int draw_index)
{
    draw_index_ = draw_index;
}

unsigned int Object::GetDrawIndex()
{
    return draw_index_;
}

/* Keeps track of the context for copying the children of one node to the
 * children of another. This data structure is meant to be used recursively
 * in the Object::deepCopy() function and instances are meant to be pushed
 * onto a stack. The iterator variable keeps track of which child is being 
 * copied. */
struct DeepCopyStackFrame
{
    Object* original;
    Object* copy;
    std::vector<Object*>::const_iterator iterator;
    DeepCopyStackFrame(Object* original, Object* copy)
        : original(original), copy(copy), 
        iterator(original->GetChildren().begin()) {}
};

Object* Object::DeepCopy()
{
    // Create a stack to hold stack frames for "recursive" tree traversal.
    std::stack<DeepCopyStackFrame> stack;
    Object* original_root = this;
    Object* copy_root = new Object(original_root);
    // First stack frame involves the root node and its copy
    stack.push(DeepCopyStackFrame(original_root, copy_root));

    /* Loop until all descendents are iterated over and copied and stack size
     * becomes zero. */
    while (stack.size() > 0)
    {
        /* View the top of the stack to deal with the current deepest
         * ongoing iteration. */
        DeepCopyStackFrame& frame = stack.top();
        /* Continue iterating until all children in this node are
         * accounted for. */
        if (frame.iterator != frame.original->children_.end())
        {
            // Shallow copy the current child.
            Object* original_child = *frame.iterator;
            Object* copy_child = new Object(original_child);
            // Replicate structure of original node and its children.
            frame.copy->children_.push_back(copy_child);
            /* Don't forget to increment iterator to track iteration at this 
             * level since the loop will jump between tree levels as deeper 
             * descendents are discovered. */
            frame.iterator++;
            /* A new stack frame will take care of any children that this
             * node has. */
            stack.push(DeepCopyStackFrame(original_child, copy_child));
        }
        /* Iterator variable reached the end, remove stack frame and continue 
         * to next adjacent child. Nodes that have no children will reach this 
         * else clause immediately. */
        else
        {
            stack.pop();
        }
    }

    return copy_root;
}

void Object::SortChildrenByDrawIndex()
{
    // Base for radix sort (10 = decimal, 16 = hexadecimal etc).
    const int kBase = 10;
    // Number of digits to process for each draw index.
    const int kDigits = 8;

    /* Reusable array that keeps track of the quantity of digits of every
     * magnitude for each index digit. In base ten for example, the number of
     * fives found in the ones place for each index will be counted, along with
     * every other digit from 0-9. This array will be reused for each place
     * (e.g. ones, tens, hundreds places). Make room for one extra count slot
     * that is unused since final count array will need to be shifted right by
     * one. */
    int digit_counts[kBase + 1];
    // Divide will be 10^d for each iteration of the outermost loop.
    int divide = 1;
    // Temporary storage for each intermediate counting sort.
    std::vector<Object*> count_sort_storage(children_.size(), nullptr);

    // Iterate for every digit (e.g. base 10 = ones, tens, hundreds)
    for (int d = 0; d < kDigits; d++)
    {
        // Reset digit counts to zero for each new iteration
        std::fill(digit_counts, digit_counts + kBase + 1, 0);
        // Count the number of digits for each draw index at current place
        for (auto it = children_.begin(); it != children_.end(); it++)
        {
            Object* child = *it;
            // Shift 'divide' decimal places and isolate the digit with modulo
            int digit_index = 1 + child->draw_index_ / divide % kBase;
            // Count the corresponding digit
            digit_counts[digit_index]++;
        }
        /* Accumulate digit count sums toward the right to keep track of
         * starting indices for each digit. */
        for (int i = 2; i < kBase; i++)
        {
            /* Compute offsets or starting indices.
             * e.g. the number of zeros tells the starting position of ones,
             * and the number of zeros AND ones tells the starting position
             * of twos. */
            digit_counts[i] = digit_counts[i - 1] + digit_counts[i];
        }
        // Store objects in proper slot according to digit in current place
        for (auto it = children_.begin(); it != children_.end(); it++)
        {
            Object* child = *it;
            int digit_index = child->draw_index_ / divide % kBase;
            count_sort_storage.at(digit_counts[digit_index]) = child;
            // Update location of starting index for this digit
            digit_counts[digit_index]++;
        }
        /* Swap temporary storage with children to obtain sorted array for
         * next iteration. */
        std::swap(children_, count_sort_storage);
        /* Next iteration, need to shift one more place to acquire next more
         * significant digit. */
        divide *= kBase;
    }
}

Object* Object::Collapse()
{
    Object* root = this->DeepCopy();

    // Create a single list by which to collapse the entire tree hierarchy.
    std::vector<Object*> object_list;
    // Add the root node as the first element in the list.
    object_list.push_back(root);

    /* Create a lambda function that will recursively add descendent nodes
     * to the list. */
    std::function<void(std::vector<Object*>&, Object*, glm::mat4)> add_children;
    /* objectList: Reference to top-level list to collect all descendent nodes.
     * parent: The node whose direct children will be iterated over in the 
     *         current function call.
     * combinedTransform: Combined transform of all ancestor nodes. */
    add_children = [&add_children](std::vector<Object*>& object_list, 
                                 Object* parent, 
                                 glm::mat4 combined_transform)
    {
        const std::vector<Object*>& children = parent->children_;
        for (auto it = children.begin(); it != children.end(); it++)
        {
            /* Combine all parent transforms with this child object's
             * transform so it can exist with the same orientation in 
             * space without a parent transform. */
            glm::mat4 transform = combined_transform * (*it)->transform_;
            (*it)->transform_ = transform;
            // Store this child in the top-level list
            object_list.push_back(*it);
            // Recurse with this child's children.
            add_children(object_list, *it, transform);
        }
        // Destroy previous hierarchy associations.
        parent->RemoveAllChildren();
    };

    // Make actual call to recursive lambda function.
    add_children(object_list, root, transform_);

    // Create new object to hold the collapsed list of objects.
    Object* group_object = new Object();
    group_object->children_ = object_list;

    return group_object;
}

// END ENHANCEMENTS
//*****************************************************************************

void Object::Draw(glm::mat4 parent_transform)
{
    glm::mat4 combined = parent_transform * transform_;
    /* Draw this model with this transformedand inherited transform
     * using texture */
    if (model_ != nullptr)
    {
        /* Get OpenGL shader currently in use so that the useTexture uniform
         * can be supplied */
        GLint shader = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shader);
        if (texture_ != nullptr)
        {
            texture_->Use("uTexture");
            model_->Draw(combined);
            texture_->Unuse();
        }
        else
        {
            model_->Draw(combined);
        }
    }
    // Automatically sort children by their draw index with a radix sort
    SortChildrenByDrawIndex();
    // Repeat process recursively for all children
    for (auto it = children_.begin(); it != children_.end(); it++)
    {
        if (*it != nullptr)
        {
            (*it)->Draw(combined);
        }
    }
}
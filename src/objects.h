#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "defines.h"
#include "descriptors/generic.h"
#include "descriptors/grid.h"
#include "vao.h"

struct Transform
{
    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 scale;

    inline void reset()
    {
        rotation = glm::identity<glm::quat>();
        scale = glm::vec3(1.0f);
        position = glm::vec3(0.0f);
    }

    inline glm::mat4 as_mat4()
    {
        auto result = glm::toMat4(rotation);
        result = glm::translate(result, position);
        result = glm::scale(result, scale);
        return result;
    }

    inline void translate(glm::vec3 v)
    {
        position += v * rotation;
    }
};

struct Camera
{
    Transform transform;
    float fov;
    float clip_near;
    float clip_far;
    float viewport_width;
    float viewport_height;

    inline glm::mat4 get_view()
    {
        return transform.as_mat4();
    }
    inline glm::mat4 get_projection()
    {
        return glm::perspective(fov, viewport_width / viewport_height, clip_near, clip_far);
    }

    void look_at(glm::vec3 point);

    inline void rotate(float angle, glm::vec3 axis) 
    { 
        transform.rotation = transform.rotation * glm::normalize(glm::angleAxis(angle, axis * transform.rotation)); 
    }
	inline void yaw(float angle) 
    { 
        rotate(angle, {0.0f, 1.0f, 0.0f}); 
    }
	inline void pitch(float angle) 
    { 
        rotate(angle, {1.0f, 0.0f, 0.0f}); 
    }
	inline void roll(float angle)
    { 
        rotate(angle, {0.0f, 0.0f, 1.0f}); 
    }
};

struct Pawn
{
    Pawn_Vao* vao;
    Transform transform;
    bool is_visible;
    s32 num_visible_triangles;

    void draw();
};

struct Grid
{
    Grid_Vao* vao;
    Transform transform;
    bool is_visible;

    void draw();
};
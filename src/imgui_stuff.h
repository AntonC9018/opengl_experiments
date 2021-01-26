#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/quaternion.hpp>
#include "rgb.h"

struct Imgui_Data
{
    RGBA background_color;
    float pawn_scale;
    int num_triangles;
    int max_triangles;
    bool show_grid;
    bool show_pawn;
    float fov;               
    glm::quat pawn_rotation; 
    float distance_to_pawn;  
};

void configure_imgui(GLFWwindow *window);
void do_imgui_stuff(Imgui_Data *imgui_data);
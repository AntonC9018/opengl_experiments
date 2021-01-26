#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/quaternion.hpp>
#include "objects.h"
#include "rgb.h"

struct Imgui_Data
{
    RGBA background_color;
    Pawn* pawn;
    Grid* grid;
    Camera* camera;
};

void configure_imgui(GLFWwindow *window);
void do_imgui_stuff(Imgui_Data *imgui_data);
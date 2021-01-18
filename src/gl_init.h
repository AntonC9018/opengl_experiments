#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window_Data
{
    int width;
    int height;
    const char* program_name;
};
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
GLFWwindow* init_gl_with_window(Window_Data initial_data);
inline int sum(int nums[], int count);
inline void setup_float_attribs(int lengths[], int count);
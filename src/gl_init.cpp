#include "gl_init.h"
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    camera.viewport_width = width;
    camera.viewport_height = height;
    glViewport(0, 0, width, height);
}

GLFWwindow* init_gl_with_window(Window_Data initial_data)
{
    if (!glfwInit())
    {
        fputs("[ERROR] Couldn't initialize GLFW", stderr);
        return 0;
    }
    else
    {
        puts("[INFO] GLFW initialized");
    }

    // setup GLFW window
    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GL 3.0 + GLSL 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    float highDPIscaleFactor = 1.0;
    // if it's a HighDPI monitor, try to scale everything
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1 || yscale > 1)
    {
        highDPIscaleFactor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }

    GLFWwindow *window = glfwCreateWindow(initial_data.width, initial_data.height, initial_data.program_name, NULL, NULL);
    if (!window)
    {
        fputs("[ERROR] Couldn't create a GLFW window\n", stderr);
        return 0;
    }
    // watch window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    // VSync
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fputs("[ERROR] Couldn't initialize GLAD", stderr);
        return 0;
    }
    else
    {
        puts("[INFO] GLAD initialized");
    }

    printf("[INFO] OpenGL from glad %i.%i\n", GLVersion.major, GLVersion.minor);

    int actual_window_width, actual_window_height;
    glfwGetWindowSize(window, &actual_window_width, &actual_window_height);
    glViewport(0, 0, actual_window_width, actual_window_height);
    camera.viewport_height = actual_window_height;
    camera.viewport_width = actual_window_width;
    
    return window;
}

inline
int sum(int nums[], int count)
{
    int s = 0;
    for (int i = 0; i < count; i++)
    {
        s += nums[i];
    }
    return s;
}

extern inline
void setup_float_attribs(int lengths[], int count)
{
    int stride = sum(lengths, count);
    float* offset = 0;
    for (int i = 0; i < count; i++)
    {
        glVertexAttribPointer(i, lengths[i], GL_FLOAT, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(i);
        offset += lengths[i];
    };
}
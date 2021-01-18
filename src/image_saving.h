#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Images (use one of these)
#define USE_FREE_IMAGE false

#if USE_FREE_IMAGE
    // Required to be linked both statically and dynamically in order to work, for some reason.
    #include <FreeImage.h>
#else
    #include <stb/stb_image.h>
    #include <stb/stb_image_write.h>
#endif

void do_image_stuff(GLFWwindow *window);

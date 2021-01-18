#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image_saving.h"
#include <stdio.h>

void do_image_stuff(GLFWwindow *window)
{
    // Save a red background as a bmp file
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow *offscreen_window = glfwCreateWindow(150, 150, "", NULL, NULL);
    glfwMakeContextCurrent(offscreen_window);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Make the BYTE array, factor of 3 because it's RBG.
    BYTE *pixels = (BYTE *)malloc(3 * 150 * 150);

#if USE_FREE_IMAGE
    {
        // Save the buffer in memory as BGR and not RGB, since B and R get swapped when saving as bmps.
        // Found out this has to do with endianness, which makes sense. The strange thing is that
        // changing blue/red channel masks 0xFF0000 vs 0x0000FF doesn't change the output, neither does
        // any combination of them make any difference. Is it a bug, or am I missing something?
        glReadPixels(0, 0, window_width, window_height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

        // Convert to FreeImage format & save to file
        FIBITMAP *image = FreeImage_ConvertFromRawBits(pixels, window_width, window_height,
                                                        3 * window_width, 24, 0, 0, 0, false);
        FreeImage_Save(FIF_BMP, image, "out1.bmp", 0);
        // Free resources
        FreeImage_Unload(image);
    }
#else
    {
        // However, this one works right with RGB!
        glReadPixels(0, 0, 150, 150, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        // This writes a weird image for some reason, there are no docs on this function and the
        // source code is too cryptic. So I guess I'm just not going to use it.
        // New Info: the `comp` parameter means the number of channels, which should be 3. It works!
        stbi_write_bmp("out1.bmp", 150, 150, 3, pixels);

        // This also works
        // stbi_write_png("out1.png", window_width, window_height, 3, pixels, window_width * 3);
    }
#endif

    free(pixels);

    glfwDestroyWindow(offscreen_window);
    glfwMakeContextCurrent(window);
}

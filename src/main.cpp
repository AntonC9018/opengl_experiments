// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// ImGui
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// C stuff
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
// Images (use one of these)
#define USE_FREE_IMAGE true
#if USE_FREE_IMAGE
    // Required to be linked both statically and dynamically in order to work, for some reason.
    #include <FreeImage.h>
#else
    #define STB_IMAGE_IMPLEMENTATION
    #include <stb_image.h>
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include <stb_image_write.h>
#endif
// My stuff
#include "rgb.h"

const char *program_name = "GLFW window";
const char *glsl_version = "#version 330";
int32_t window_width = 1200;
int32_t window_height = 800;
RGBA background_color{1.0f, 0.0f, 0.0f, 1.0f};

struct Imgui_Data
{
    bool show_demo_window;
    bool show_another_window;
    float some_float;
    int some_counter;
} imgui_data = { true, true, 0.0f, 0 };

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void teardown(GLFWwindow *window);
void do_imgui_stuff(Imgui_Data *imgui_data);

int main()
{
    if (!glfwInit())
    {
        fputs("[ERROR] Couldn't initialize GLFW", stderr);
        return -1;
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

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, program_name, NULL, NULL);
    if (!window)
    {
        fputs("[ERROR] Couldn't create a GLFW window\n", stderr);
        teardown(NULL);
        return -1;
    }
    // watch window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    // VSync
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fputs("[ERROR] Couldn't initialize GLAD", stderr);
        teardown(window);
        return -1;
    }
    else
    {
        puts("[INFO] GLAD initialized");
    }

    printf("[INFO] OpenGL from glad %i.%i\n", GLVersion.major, GLVersion.minor);
    {
        int actual_window_width, actual_window_height;
        glfwGetWindowSize(window, &actual_window_width, &actual_window_height);
        glViewport(0, 0, actual_window_width, actual_window_height);
    }

    

    if (0)
    {
        // Save a red background as a bmp file
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        GLFWwindow *offscreen_window = glfwCreateWindow(window_width, window_height, "", NULL, NULL);
        glfwMakeContextCurrent(offscreen_window);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Make the BYTE array, factor of 3 because it's RBG.
        BYTE *pixels = (BYTE*)malloc(3 * window_width * window_height);

        // Save the buffer in memory as BGR and not RGB, since B and R get swapped when saving as bmps.
        // Found out this has to do with endianness, which makes sense. The strange thing is that 
        // changing blue/red channel masks 0xFF0000 vs 0x0000FF doesn't change the output, neither does 
        // any combination of them make any difference. Is it a bug, or am I missing something?
        glReadPixels(0, 0, window_width, window_height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

#if USE_FREE_IMAGE
        {
            // Convert to FreeImage format & save to file
            FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, window_width, window_height, 
                3 * window_width, 24, 0, 0, 0, false);
            FreeImage_Save(FIF_BMP, image, "D:/Coding/C++/out1.bmp", 0);
            // Free resources
            FreeImage_Unload(image);
        }
#else
        {
            // This writes a weird image for some reason, there are no docs on this function and the 
            // source code is too cryptic. So I guess I'm just not going to use it.
            stbi_write_bmp("D:/Coding/C++/out1.bmp", window_width, window_height, 1, pixels);
        }
#endif

        free(pixels);

        glfwDestroyWindow(offscreen_window);
        glfwMakeContextCurrent(window);
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    _chdir("W:\\opengl_test\\assets");
    ImFont *some_font;
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/ProggyTiny.ttf", 10.0f);
    some_font = io.Fonts->AddFontFromFileTTF("fonts/ArialUnicodeMS.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(some_font != NULL);

    // --- rendering loop
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // do_imgui_stuff(&imgui_data);

        glViewport(0, 0, window_width, window_height);
        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    teardown(window);

    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void teardown(GLFWwindow *window)
{
    if (window != NULL)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void do_imgui_stuff(Imgui_Data *imgui_data)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (imgui_data->show_demo_window)
        ImGui::ShowDemoWindow(&imgui_data->show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &imgui_data->show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &imgui_data->show_another_window);

        ImGui::SliderFloat("float", &imgui_data->some_float, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *)&background_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            imgui_data->some_counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", imgui_data->some_counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (imgui_data->show_another_window)
    {
        ImGui::Begin("Another Window", &imgui_data->show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            imgui_data->show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
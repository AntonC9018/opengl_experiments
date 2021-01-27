// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// C stuff
#include <direct.h>
#include <stdio.h>
// My stuff
#include "defines.h"
#include "rgb.h"
#include "imgui_stuff.h"
#include "gl_init.h"
#include "image_saving.h"
#include "objects.h"

extern Camera camera;
internal bool first_mouse = true;
internal bool respond_to_mouse = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    local_persistent glm::vec2 last; 

    if (!respond_to_mouse)
    {
        return;
    }
    if (first_mouse)
    {
        last.x = (float)xpos;
        last.y = (float)ypos;
        first_mouse = false;
    }
    float xoffset = (float)xpos - last.x;
    float yoffset = (float)ypos - last.y; 
    last.x = (float)xpos;
    last.y = (float)ypos;

    float sensitivity = 0.002f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // This introduces roll due to floating point error. TODO: Use euler angles?
    camera.yaw(xoffset);
    camera.pitch(yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    const float zoom_speed = 0.1f;
    camera.fov = glm::clamp(camera.fov - (float)yoffset * (float)zoom_speed, 
        0.05f, glm::three_over_two_pi<float>());
}

void move_camera_based_on_input(GLFWwindow* window, Camera* camera)
{
    const float camera_speed = 0.05f;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->transform.translate(glm::vec3(0.0f, 0.0f, camera_speed));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->transform.translate(glm::vec3(camera_speed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->transform.translate(glm::vec3(0.0f, 0.0f, -camera_speed));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->transform.translate(glm::vec3(-camera_speed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->transform.translate(glm::vec3(0.0f, camera_speed, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->transform.translate(glm::vec3(0.0f, -camera_speed, 0.0f));

    local_persistent bool just_started_pressing = false;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && just_started_pressing)
    {
        if (respond_to_mouse)
        {
            respond_to_mouse = false;
            first_mouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            respond_to_mouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        just_started_pressing = false;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
    {
        just_started_pressing = true;
    }
}

int main()
{
    _chdir("W:\\opengl_test\\assets");

    GLFWwindow* window = init_gl_with_window({ 800, 800, "GLFW window" });
    configure_imgui(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    Camera_View_Projection_Block vp_block;
    vp_block.create(1);

    Pawn_Vao pawn_vao;
    pawn_vao.load_model();
    pawn_vao.load_shaders(vp_block);

    Pawn pawn;
    pawn.vao = &pawn_vao;
    pawn.is_visible = true;
    pawn.transform.reset();
    pawn.num_visible_triangles = pawn_vao.num_triangles;

    Grid_Vao grid_vao;
    grid_vao.board_dimension = 8.0f;
    grid_vao.load_model();
    grid_vao.load_shaders(vp_block);

    Grid grid;
    grid.vao = &grid_vao;
    grid.is_visible = false;
    grid.transform.reset();
    grid.transform.position = { 0.0f, 0.0f, 0.0f };

    // Camera camera; I have defined a global camera in window
    camera.fov = glm::radians(50.0f);
    camera.clip_near = 0.1f;
    camera.clip_far = 1000.0f;
    camera.transform.scale = glm::vec3(1.0f);
    camera.transform.position = glm::vec3(0.0f, -5.0f, 0.0f);
    camera.look_at(pawn.transform.position);

    Imgui_Data imgui_data;
    imgui_data.background_color = {0.2f, 0.3f, 0.2f, 1.0f};
    imgui_data.camera = &camera;
    imgui_data.pawn = &pawn;
    imgui_data.grid = &grid;

    // The Render Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(
            imgui_data.background_color.r, 
            imgui_data.background_color.g, 
            imgui_data.background_color.b, 
            imgui_data.background_color.a
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        move_camera_based_on_input(window, &camera);
        
        vp_block.bind();
        vp_block.view(camera.get_view());
        vp_block.projection(camera.get_projection());

        pawn.transform.rotation = glm::normalize(pawn.transform.rotation);
        pawn.draw();

        grid.draw();

        do_imgui_stuff(&imgui_data);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();

    return 0;
}

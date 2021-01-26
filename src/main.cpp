// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
// C stuff
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
// My stuff
#define internal static
#define local_persistent static
#include "rgb.h"
#include "shader.h"
#include "descriptors/generic.h"
#include "descriptors/grid.h"
#include "imgui_stuff.h"
#include "gl_init.h"
#include "image_saving.h"
#include "vao.h"

int main()
{
    _chdir("W:\\opengl_test\\assets");

    GLFWwindow* window = init_gl_with_window({ 800, 800, "GLFW window" });
    configure_imgui(window);

    Camera_View_Projection_Block vp_block;
    vp_block.create(1);

    float board_dimension = 8.0f;
    uint32_t board_vao = create_board_vao(board_dimension);
    Grid_Program board_program;
    board_program.id = create_shader_program("shader_src/grid.vs", "shader_src/grid.fs");
    board_program.query_locations();
    board_program.Camera_View_Projection_block(vp_block);

    Imgui_Data imgui_data;
    imgui_data.background_color = {0.2f, 0.3f, 0.2f, 1.0f};
    imgui_data.distance_to_pawn = 5.0f;
    imgui_data.show_grid = false;
    imgui_data.show_pawn = true;
    imgui_data.fov = 50.0f;
    imgui_data.pawn_rotation = glm::identity<glm::quat>();
    imgui_data.pawn_scale = 1;

    uint32_t pawn_vao = create_pawn_vao(&imgui_data.max_triangles);
    imgui_data.num_triangles = imgui_data.max_triangles;
    Generic_Program pawn_program;
    pawn_program.id = create_shader_program("shader_src/generic.vs", "shader_src/generic.fs");
    pawn_program.query_locations();
    pawn_program.Camera_View_Projection_block(vp_block);

    // The Render Loop
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glClearColor(
            imgui_data.background_color.r, 
            imgui_data.background_color.g, 
            imgui_data.background_color.b, 
            imgui_data.background_color.a
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        auto view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -imgui_data.distance_to_pawn));
        auto projection = glm::perspective(glm::radians(imgui_data.fov), 1.0f, 0.1f, 100.0f);
        
        vp_block.bind();
        vp_block.view(view);
        vp_block.projection(projection);

        auto light_position = glm::vec3(0.0f, 10.0f, 0.0f);

        if (imgui_data.show_grid)
        {
            glBindVertexArray(board_vao);

            auto board_model = glm::toMat4(imgui_data.pawn_rotation);
            board_model = glm::translate(board_model, glm::vec3(-0.5, -0.5, 0));
            board_model = glm::scale(board_model, glm::vec3(1/board_dimension));
            
            board_program.use();
            board_program.uniforms(board_dimension, light_position, board_model);
            
            glDisable(GL_CULL_FACE); 
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6 for 2 triangles, 3 indices per each
        }

        if (imgui_data.show_pawn)
        {
            glBindVertexArray(pawn_vao);
            
            imgui_data.pawn_rotation = glm::normalize(imgui_data.pawn_rotation);
            auto pawn_model = glm::toMat4(imgui_data.pawn_rotation);
            pawn_model = glm::scale(pawn_model, glm::vec3(imgui_data.pawn_scale));

            pawn_program.use();
            pawn_program.uniforms(light_position, pawn_model);

            glEnable(GL_CULL_FACE); 
            glDrawElements(GL_TRIANGLES, imgui_data.num_triangles * 3, GL_UNSIGNED_INT, 0);
        }

        imgui_data.pawn_rotation.x += 0.001f;

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

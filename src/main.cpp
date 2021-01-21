// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
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
#include "imgui_stuff.h"
#include "gl_init.h"
#include "image_saving.h"
#include "vao.h"

int main()
{
    _chdir("W:\\opengl_test\\assets");

    GLFWwindow* window = init_gl_with_window({ 800, 800, "GLFW window" });
    configure_imgui(window);

    float board_dimension = 8.0f;
    uint32_t board_vao = create_board_vao(board_dimension);
    uint32_t board_shader_program = create_shader_program("shader_src/grid.vs", "shader_src/grid.fs");

    Imgui_Data imgui_data = {{0.2f, 0.3f, 0.2f, 1.0f}, 0.5f, 1, 0, false, true, 5.0f, glm::identity<glm::quat>(), 50.0f};

    uint32_t pawn_vao = create_pawn_vao(imgui_data.max_triangles);
    uint32_t pawn_shader_program = create_shader_program("shader_src/generic.vs", "shader_src/generic.fs");
    imgui_data.num_triangles = imgui_data.max_triangles;    

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

        imgui_data.pawn_rotation = glm::normalize(imgui_data.pawn_rotation);
        auto pawn_model = glm::toMat4(imgui_data.pawn_rotation);
        pawn_model = glm::scale(pawn_model, glm::vec3(imgui_data.pawn_scale));

        auto board_model = glm::toMat4(imgui_data.pawn_rotation);
        board_model = glm::translate(board_model, glm::vec3(-0.5, -0.5, 0));
        board_model = glm::scale(board_model, glm::vec3(1/board_dimension));
        
        auto view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -imgui_data.distance_to_pawn));

        auto projection = glm::perspective(glm::radians(imgui_data.fov), 1.0f, 0.1f, 100.0f);
        auto light_position = glm::vec3(0.0f, 10.0f, 0.0f);


        if (imgui_data.show_grid)
        {
            glUseProgram(board_shader_program);

            auto model_loc = glGetUniformLocation(board_shader_program, "model");
            auto view_loc = glGetUniformLocation(board_shader_program, "view");
            auto projection_loc = glGetUniformLocation(board_shader_program, "projection");
            auto light_position_loc = glGetUniformLocation(board_shader_program, "light_position");
            auto board_dimension_loc = glGetUniformLocation(board_shader_program, "board_dimension");

            glUniform1f(board_dimension_loc, board_dimension); 
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float*)&board_model); 
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)&view); 
            glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float*)&projection);
            glUniform3fv(light_position_loc, 1, (float*)&light_position);

            glBindVertexArray(board_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6 for 2 triangles, 3 indices per each
        }

        if (imgui_data.show_pawn)
        {
            glUseProgram(pawn_shader_program);

            auto model_loc = glGetUniformLocation(pawn_shader_program, "model");
            auto view_loc = glGetUniformLocation(pawn_shader_program, "view");
            auto projection_loc = glGetUniformLocation(pawn_shader_program, "projection");
            auto light_position_loc = glGetUniformLocation(pawn_shader_program, "light_position");

            glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float*)&pawn_model); 
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)&view); 
            glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float*)&projection);
            glUniform3fv(light_position_loc, 1, (float*)&light_position);

            glBindVertexArray(pawn_vao);
            glDrawElements(GL_TRIANGLES, imgui_data.num_triangles * 3, GL_UNSIGNED_INT, 0);
            // glDrawArrays(GL_TRIANGLES, 0, imgui_data.num_triangles * 3);
        }

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

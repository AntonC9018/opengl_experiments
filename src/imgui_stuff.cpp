#include "imgui_stuff.h"

const char *glsl_version = "#version 330";

void configure_imgui(GLFWwindow *window)
{
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

    ImFont *some_font;
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../assets/fonts/ProggyTiny.ttf", 10.0f);
    some_font = io.Fonts->AddFontFromFileTTF("fonts/ArialUnicodeMS.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(some_font != NULL);
}


void do_imgui_stuff(Imgui_Data *imgui_data)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // if (imgui_data->show_demo_window)
    //     ImGui::ShowDemoWindow(&imgui_data->show_demo_window);

    {
        ImGui::Begin("Chess");
        ImGui::Text("Average FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Checkbox("Show grid", &imgui_data->grid->is_visible);
        ImGui::Checkbox("Show pawn", &imgui_data->pawn->is_visible);
        ImGui::SliderFloat3("Pawn Scale", &imgui_data->pawn->transform.scale[0], 0.0f, 1.0f);
        ImGui::SliderFloat4("Pawn Rotation: ", &imgui_data->pawn->transform.rotation[0], -1.0f, 1.0f);
        ImGui::SliderInt("Pawn triangle count", &imgui_data->pawn->num_visible_triangles, 1, imgui_data->pawn->vao->num_triangles);
        // ImGui::SliderFloat("Distance to pawn", &imgui_data->distance_to_pawn, 0.0f, 15.0f);
        // ImGui::SliderFloat("FOV", &imgui_data->camera->fov, 0.0f, 180.0f);
        ImGui::ColorEdit3("Clear Color", &imgui_data->background_color.r);
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
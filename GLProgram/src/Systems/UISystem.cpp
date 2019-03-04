#include "UISystem.h"
#include "Global.h"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "PhysicManager.h"

UISystem::UISystem()
{
}

UISystem::~UISystem()
{
}

void UISystem::Init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, false);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
}

void UISystem::Update()
{
    ImGui_ImplGlfwGL3_NewFrame();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Debug Window");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("DEBUG DRAW", &DEBUG_DRAW_ENABLE);      // Edit bools storing our window open/close state
        ImGui::Checkbox("TEXTURE", &TEXTURE_DRAW_ENABLE);      // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);


        ImGui::Text("Potential Contacts Size : %d", PhysicManager::GetPotentialContatctListSize());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void UISystem::Terminate()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

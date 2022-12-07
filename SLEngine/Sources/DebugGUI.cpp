#include "SLEngine/Internal/DebugGUI.h"
#include "SLEngine/DebugGUI.h"
#include "SLEngine/Debug.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_bgfx.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

namespace SLEngine
{
    namespace DebugGUI
    {
        bool show_ball = true;
	    bool show_floor = true;
	    ImVec4 ballColor = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

        struct DebugGUIFloatData
        {
            DebugGUIFloat value;
            std::string message;
            uint32_t handler;
        };

        std::array<DebugGUIFloatData, 256> g_guiFloats;

        void Initialize(GLFWwindow* window)
        {
            // Setup Dear ImGui context
	        IMGUI_CHECKVERSION();
	        ImGui::CreateContext();
	        ImGuiIO &io = ImGui::GetIO();
	        (void)io;
	        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	        ImGui::StyleColorsDark();

	        // Setup Platform/Renderer backends
	        ImGui_ImplGlfw_InitForOther(window, true);

            int width, height;
			glfwGetWindowSize(window, &width, &height);
            ImGui_ImplBGFX_Init(width , height);

            std::memset((void*)&g_guiFloats[0], 0, sizeof(DebugGUIFloatData) * g_guiFloats.size());
        }

        void Draw()
        {
            // Start the Dear ImGui frame
		    ImGui_ImplBGFX_NewFrame();
		    ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

			static float ballSize = 1.0f;

			ImGui::Begin("Scene Editor Example");

			ImGui::Text("Use this controllers to change values in the Babylon scene.");

            ImGui::Checkbox("Show ball", &show_ball);
            ImGui::Checkbox("Show floor", &show_floor);
            ImGui::SliderFloat("Ball Size", &ballSize, 1.0f, 10.0f);
            ImGui::ColorEdit3("Ball Color", (float *)&ballColor);
            ImGui::Button("Resume");

			ImGui::SameLine();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

            ImGui::Begin("Game Debug");

            for (auto& guiFloat : g_guiFloats)
            {
                if(guiFloat.handler != 0)
                    ImGui::InputFloat(guiFloat.message.c_str(), &guiFloat.value.value);
            }

            ImGui::End();
            

			ImGui::Render();
			ImGui_ImplBGFX_RenderDrawData(ImGui::GetDrawData());
        }
        
        void Shutdown()
        {
            ImGui_ImplBGFX_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        } 
        
        DebugGUIFloat* CreateGUIFloat(float value, std::string text)
        {
            for (size_t i = 0; i < g_guiFloats.size(); i++)
            {
                if (g_guiFloats[i].handler == 0)
                {
                    g_guiFloats[i] = DebugGUIFloatData{};
                    g_guiFloats[i].value.value = value;
                    g_guiFloats[i].message = text;
                    g_guiFloats[i].handler = 1;

                    return &g_guiFloats[i].value;
                }
            }

            SLENGINE_ERROR("More than 256 DebugGUIFloat created.");

            return nullptr;
        }

        void DeleteGUIFloat(DebugGUIFloat& debugFloat)
        {

        }
    }
}
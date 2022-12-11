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
            float value;
            std::string message;
            uint32_t handler;
            FloatWidget widget;
            float maxSlider;
            float minSlider;
        };

        std::array<DebugGUIFloatData, 256> g_guiFloats;

        void Initialize(GLFWwindow *window)
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
            ImGui_ImplBGFX_Init(width, height);

            std::memset((void *)&g_guiFloats[0], 0, sizeof(DebugGUIFloatData) * g_guiFloats.size());
        }

        void DrawFloatInput(DebugGUIFloatData &floatGUI)
        {
            if (floatGUI.widget == FloatWidget::InputField)
            {
                ImGui::InputFloat(floatGUI.message.c_str(), &floatGUI.value);
            }
            else if (floatGUI.widget == FloatWidget::Slider)
            {
                ImGui::SliderFloat(floatGUI.message.c_str(), &floatGUI.value, floatGUI.minSlider, floatGUI.maxSlider);
            }
            else if (floatGUI.widget == FloatWidget::Drag)
            {
                ImGui::DragFloat(floatGUI.message.c_str(), &floatGUI.value, floatGUI.minSlider, floatGUI.maxSlider);
            }
        }

        float g_menuWidth = 250;

        void DrawLeftMenu(ImGuiViewport *viewport)
        {
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(ImVec2(g_menuWidth, viewport->Size.y));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            if (ImGui::Begin("DockSpace Demo", 0, flags))
            {
                ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable;
                

                if (ImGui::CollapsingHeader("Engine Data"))
                {
                    ImGui::Text("Frame time: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                }

                if (ImGui::CollapsingHeader("Game Data"))
                {
                    for (auto &guiFloat : g_guiFloats)
                    {
                        if (guiFloat.handler != 0)
                            DrawFloatInput(guiFloat);
                    }
                }

                g_menuWidth = ImGui::GetWindowWidth();

                ImGui::End();
            }

            ImGui::PopStyleVar();
        }

        void Draw()
        {
            // Start the Dear ImGui frame
            ImGui_ImplBGFX_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

            ImGuiViewport *viewport = ImGui::GetMainViewport();
            DrawLeftMenu(viewport);

            ImGui::Render();
            ImGui_ImplBGFX_RenderDrawData(ImGui::GetDrawData());
        }

        void Shutdown()
        {
            ImGui_ImplBGFX_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }

        DebugGUIFloat CreateGUIFloat(float value, std::string text, FloatWidget widget, float sliderMin, float sliderMax)
        {
            for (size_t i = 0; i < g_guiFloats.size(); i++)
            {
                if (g_guiFloats[i].handler == 0)
                {
                    g_guiFloats[i] = DebugGUIFloatData{};
                    g_guiFloats[i].value = value;
                    g_guiFloats[i].message = text;
                    g_guiFloats[i].handler = 1;
                    g_guiFloats[i].widget = widget;
                    g_guiFloats[i].minSlider = sliderMin;
                    g_guiFloats[i].maxSlider = sliderMax;

                    return DebugGUIFloat{&g_guiFloats[i].value};
                }
            }

            SLENGINE_ERROR("More than 256 DebugGUIFloat created.");

            return DebugGUIFloat{};
        }

        void DeleteGUIFloat(DebugGUIFloat &debugFloat)
        {
        }
    }
}
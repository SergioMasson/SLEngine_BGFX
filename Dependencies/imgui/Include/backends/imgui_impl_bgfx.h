

// dear imgui: Renderer Backend for DirectX11
// This needs to be used along with a Platform Backend (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'ID3D11ShaderResourceView*' as ImTextureID. Read the FAQ about ImTextureID!
//  [X] Renderer: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this. 
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API

#include <cstdint>

IMGUI_IMPL_API bool ImGui_ImplBGFX_Init(uint32_t width, uint32_t height, float fontSize = 16.0f);
IMGUI_IMPL_API void ImGui_ImplBGFX_Shutdown();
IMGUI_IMPL_API void ImGui_ImplBGFX_NewFrame();
IMGUI_IMPL_API void ImGui_ImplBGFX_RenderDrawData(ImDrawData* draw_data);
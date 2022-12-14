cmake_minimum_required(VERSION 3.0.0)

add_library(SLEngine)

set(SLENGINE_SOURCES 
    "Sources/GameCore.cpp"
    "Sources/Input.cpp"
    "Sources/Graphics.cpp"
    "Sources/Debug.cpp"
    "Sources/DebugGUI.cpp"
    "Sources/IndexBuffer.cpp"
    "Sources/VertexBuffer.cpp"
    "Sources/VertexArray.cpp"
    "Sources/UniformBuffer.cpp")

set(SLEGINE_HEADERS
    "Include/SLEngine/GameCore.h"
    "Include/SLEngine/Input.h"
    "Include/SLEngine/Debug.h"
    "Include/SLEngine/DebugGUI.h"
    "Include/SLEngine/MeshRenderer.h"
    "InternalInclude/SLEngine/Internal/DebugGUI.h"
    "InternalInclude/SLEngine/Internal/IndexBuffer.h"
    "InternalInclude/SLEngine/Internal/VertexBuffer.h"
    "InternalInclude/SLEngine/Internal/VertexArray.h"
    "InternalInclude/SLEngine/Internal/UniformBuffer.h"
    "InternalInclude/SLEngine/Internal/Input.h"
    "InternalInclude/SLEngine/Internal/Graphics.h")

target_sources(SLEngine PRIVATE ${SLENGINE_SOURCES} ${SLEGINE_HEADERS})
target_include_directories(SLEngine PUBLIC Include)
target_include_directories(SLEngine PRIVATE InternalInclude)

if(MSVC)
    target_compile_definitions(SLEngine PUBLIC SLENGINE_PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(SLEngine PUBLIC SLENGINE_PLATFORM_OSX)
    target_link_libraries(SLEngine PUBLIC "-framework Cocoa" "-framework MetalKit" "-framework QuartzCore" "-framework CoreFoundation" "-framework CoreGraphics")
else()
    target_compile_definitions(SLEngine PUBLIC SLENGINE_PLATFORM_LINUX)
endif()

target_link_to_dependencies(SLEngine PRIVATE glfw bgfx bimg glslang SPIRV bx imgui)

if(GRAPHICS_API STREQUAL "Metal")
    target_link_to_dependencies(SLEngine PRIVATE metal-cpp metal-cpp-extensions)
endif()

if(TARGET spirv-cross-hlsl)
    target_link_to_dependencies(SLEngine PRIVATE spirv-cross-hlsl)
elseif(TARGET spirv-cross-msl)
    target_link_to_dependencies(SLEngine PRIVATE spirv-cross-msl)
else()
    target_link_to_dependencies(SLEngine PRIVATE spirv-cross-glsl)
endif()

target_compile_features(SLEngine PRIVATE cxx_std_17)
target_compile_definitions(SLEngine PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:SLENGINE_RELEASE>")
target_compile_definitions(SLEngine PRIVATE "$<$<CONFIG:DEBUG>:SLENGINE_DEBUG>")
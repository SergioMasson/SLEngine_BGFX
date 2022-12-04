cmake_minimum_required(VERSION 3.0.0)
project(bgfxGame VERSION 0.1.0)

add_subdirectory(cmake)

if(MSVC)
    set(GRAPHICS_API "D3D11")
elseif(Apple)
    set(GRAPHICS_API "Metal")
else()
    set(GRAPHICS_API "Vulkan")
endif()

add_subdirectory(Dependencies)
add_subdirectory(SLEngine)
add_subdirectory(Games)
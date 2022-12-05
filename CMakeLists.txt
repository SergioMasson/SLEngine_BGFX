cmake_minimum_required(VERSION 3.0.0)
project(bgfxGame C CXX OBJC OBJCXX)

add_subdirectory(cmake)

if(MSVC)
    set(GRAPHICS_API "D3D11")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GRAPHICS_API "Metal")
else()
    set(GRAPHICS_API "Vulkan")
endif()

message("${GRAPHICS_API}")

add_subdirectory(Dependencies)
add_subdirectory(SLEngine)
add_subdirectory(Games)
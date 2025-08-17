cmake_minimum_required(VERSION 3.20)
project(Asthrarisine LANGUAGES CXX C)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC to use /MT (static runtime)
if(MSVC)
    foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                     CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        if(${flag_var} MATCHES "/MD")
            string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
        if(${flag_var} MATCHES "/MDd")
            string(REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/dependencies/include
    ${CMAKE_SOURCE_DIR}/dependencies/imgui
    ${CMAKE_SOURCE_DIR}/Engine
)

# Source files
set(SOURCES
    dependencies/imgui/imgui.cpp
    dependencies/imgui/imgui_draw.cpp
    dependencies/imgui/imgui_impl_glfw.cpp
    dependencies/imgui/imgui_impl_opengl3.cpp
    dependencies/imgui/imgui_tables.cpp
    dependencies/imgui/imgui_widgets.cpp
    dependencies/include/glm/detail/glm.cpp
    Engine/tinygltf_loader.cpp
    glad.c
    Engine/LightManager.cpp
    Engine/Renderer/Renderer.cpp
    Engine/Renderer/src/Shader.cpp
    Main.cpp
)

set(HEADERS
    Engine/ArcBall.hpp
    Engine/ComponentManager.hpp
    Engine/Entities.hpp
    Engine/LoadModels.hpp
    Engine/Renderer/Renderer.hpp
    Engine/Renderer/src/Shader.hpp
    Engine/Renderer/Utils.hpp
    Engine/LightManager.hpp
    Engine/SceneManager.hpp
    Engine/Shape.hpp
    Engine/Shape_Library.hpp
    Engine/Transformation.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG _DEBUG IMGUI_HAS_DOCK IMGUI_HAS_VIEWPORT>
    $<$<CONFIG:Release>:NDEBUG IMGUI_HAS_DOCK IMGUI_HAS_VIEWPORT>
)

# GLFW prebuilt static lib (Option 1)
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/dependencies/include"
)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    glfw
)

# Windows Unicode
if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif()

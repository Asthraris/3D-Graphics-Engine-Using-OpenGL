cmake_minimum_required(VERSION 3.20) # you have 3.31.1, so fine
project(Asthrarisine LANGUAGES CXX C)

# --- Compiler settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # avoid MSVC-specific extensions unless needed

# --- Source files ---
set(SOURCES
    Main.cpp
    glad.c
)

# --- Create executable ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Include directories (relative, portable) ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/include
    ${CMAKE_SOURCE_DIR}/dependencies/imgui
    ${CMAKE_SOURCE_DIR}/Engine
)

# --- Library directories (relative) ---
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/lib
)

# --- Link libraries ---
target_link_libraries(${PROJECT_NAME}
    opengl32      # Windows OpenGL
    glfw3         # GLFW3 static/dll
)

# --- MSVC-specific tweaks ---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
endif()

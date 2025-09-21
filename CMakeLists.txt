cmake_minimum_required(VERSION 3.20)

project(fps)

add_executable(fps src/main.cpp src/LoadShaders.cpp include/LoadShaders.h include/stb_image.h src/initWindow.cpp src/loadTexture.cpp glad.c)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(fps PRIVATE glfw)

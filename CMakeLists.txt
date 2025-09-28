cmake_minimum_required(VERSION 3.20)

project(fps)

add_executable(fps
		src/main.cpp
		src/LoadShaders.cpp
		src/initWindow.cpp
		src/loadTexture.cpp
		src/Camera.cpp
		src/World.cpp
		glad.c)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(fps PRIVATE glfw)

target_include_directories(fps PRIVATE ${GLM_INCLUDE_DIRS})

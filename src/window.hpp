#pragma once

#include <GLFW/glfw3.h>

#include <string>

GLFWwindow* initWindow(int width, int height, std::string title);

void closeWindow(GLFWwindow* window);
#include "window.hpp"

#include <iostream>

GLFWwindow* initWindow(int width, int height, std::string title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(
        width, height, title.c_str(),
        /* fullscreen */ NULL, /*  */ NULL);
    if (window == NULL) {
        std::cout << __FILE__ << ":" << __LINE__ << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        exit(-1);;
    }
    
    glfwMakeContextCurrent(window);
    return window;
}

void closeWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
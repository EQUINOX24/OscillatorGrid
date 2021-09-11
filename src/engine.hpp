#pragma once

#include <string>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

//______________________________________________________________________________________________________________________________
#define ASSERT(x) if (!(x)) exit(-1)
// #define OPENGL_DEBUG_MODE
#ifdef OPENGL_DEBUG_MODE
    void GLClearErrors();
    bool GLLogDo(const char* function, const char* file, int line);
    #define GLDO(x)\
        GLClearErrors();\
        x;\
        ASSERT(GLLogDo(#x, __FILE__, __LINE__))
#else
    #define GLDO(x) x
#endif

//______________________________________________________________________________________________________________________________

GLFWwindow* initWindow(int width, int height, std::string title);

void closeWindow(GLFWwindow* window);

unsigned int createShaderProgram(std::string vert_src_path, std::string frag_src_path);

unsigned int loadTextureToGPU(std::string path);

//______________________________________________________________________________________________________________________________

class Renderer {
private:
    float* verts_0;
    unsigned int verts_len;
    unsigned int* inds;
    unsigned int inds_len;
    
    unsigned int stride;
    unsigned int loc_pos_0;
    unsigned int loc_pos;
    unsigned int offset_pos;
    
    unsigned int verts_size;
    
    unsigned int vao, vbo, vbo_0, ebo;
    unsigned int shader_program;
    
public:
    Renderer(float* verts_0, float* verts, unsigned int verts_len, unsigned int* inds, unsigned int inds_len);
    ~Renderer();
    
    void Draw();
};
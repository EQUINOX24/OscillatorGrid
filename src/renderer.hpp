#pragma once

#include <string>

// #define OPENGL_DEBUG_MODE

//______________________________________________________________________________________________________________________________

void initGlad();

unsigned int createShaderProgram(std::string vert_src_path, std::string frag_src_path);

unsigned int loadTextureToGPU(std::string path);

void clearWindow(float red, float green, float blue);

//______________________________________________________________________________________________________________________________

class Renderer {
private:
    float* verts_dev;
    unsigned int verts_len;
    unsigned int* inds;
    unsigned int inds_len;
    
    unsigned int stride;
    unsigned int loc_pos_dev;
    unsigned int loc_pos;
    unsigned int offset_pos;
    
    unsigned int verts_size;
    
    unsigned int vao, vbo, vbo_dev, ebo;
    unsigned int shader_program;
    
public:
    Renderer(float* verts_dev, float* verts, unsigned int verts_len, unsigned int* inds, unsigned int inds_len);
    ~Renderer();
    
    void Draw();
};
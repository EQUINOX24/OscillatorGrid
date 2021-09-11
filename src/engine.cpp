#include "engine.hpp"

// #include "stb/stb_image.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

void GLClearErrors() { while(glGetError() != GL_NO_ERROR); }

bool GLLogDo(const char* function, const char* file, int line) {
    if (GLenum error = glGetError()) {
        std::cout << file << ":" << line << " [OpenGL Error] (" << error << "): " << function << '\n';
        return false;
    }
    return true;
}

GLFWwindow* initWindow(int width, int height, std::string title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(
        width, height, title.c_str(),
        /* fullscreen */ NULL, /* vsync? */ NULL);
    if (window == NULL) {
        std::cout << __FILE__ << ":" << __LINE__ << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        exit(-1);;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    #ifdef OPENGL_DEBUG_MODE
        std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';
        int numAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
        std::cout << "Maximum number of vertex attributes supported: " << numAttributes << '\n';
    #endif
    
    return window;
}

void closeWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// _____________________________________________________________________________________________________________________________


static std::string readFrom(std::string path) {
    std::ifstream in(path, std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    std::cout << __FILE__ << ":" << __LINE__ << " Could not load file: " << path << '\n';
    exit(-1);
}
// _____________________________________________________________________________________________________________________________


static unsigned int compileShader(std::string filepath, GLenum shader_type) {
    int success, len;
    GLDO(unsigned int shader = glCreateShader(shader_type));
    std::string src = readFrom(filepath);
    const char* cstr_src = src.c_str();
    GLDO(glShaderSource(shader, 1, &cstr_src, NULL));
    GLDO(glCompileShader(shader));
    GLDO(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (success == GL_FALSE) {
        GLDO(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len));
        char* message = (char*)alloca(len * sizeof(char));
        GLDO(glGetShaderInfoLog(shader, len, &len, message));
        std::cout << __FILE__ << ":" << __LINE__ << " Failed to compile a shader" << '\n';
        std::cout << filepath << ':' << message << '\n';
        GLDO(glDeleteShader(shader));
        exit(-1);
    }
    return shader;
}

unsigned int createShaderProgram(std::string vert_src_path, std::string frag_src_path) {
    int success, length;
    unsigned int vert_shader = compileShader(vert_src_path, GL_VERTEX_SHADER);
    unsigned int frag_shader = compileShader(frag_src_path, GL_FRAGMENT_SHADER);
    GLDO(unsigned int program = glCreateProgram());
    GLDO(glAttachShader(program, vert_shader));
    GLDO(glAttachShader(program, frag_shader));
    GLDO(glLinkProgram(program));
    GLDO(glValidateProgram(program));
    GLDO(glGetProgramiv(program, GL_VALIDATE_STATUS, &success));
    
    GLDO(glDeleteShader(vert_shader));
    GLDO(glDeleteShader(frag_shader));
    
    if (success == GL_FALSE) {
        GLDO(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLDO(glGetProgramInfoLog(program, length, &length, message));
        std::cout << __FILE__ << ":" << __LINE__ << "Could not link a shader program" << '\n';
        std::cout << message << '\n';
        GLDO(glDeleteProgram(program));
        exit(-1);
    }
    return program;
}

// unsigned int loadTextureToGPU(std::string path) {
//     int width, height, channels;
//     unsigned int id;
//     unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
//     if(!data) {std::cout << "Failed to load " << path << '\n'; exit(-1);}
// 
//     GLDO(glGenTextures(1, &id));
//     GLDO(glBindTexture(GL_TEXTURE_2D, id));
//     GLDO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
//     GLDO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
//     GLDO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//     GLDO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
// 
//     GLDO(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
//     GLDO(glGenerateMipmap(GL_TEXTURE_2D));
// 
//     stbi_image_free(data);
//     GLDO(glBindTexture(GL_TEXTURE_2D, 0));
//     return id;
// }

Renderer::Renderer(float* verts_0, float* verts, unsigned int verts_len, unsigned int* inds, unsigned int inds_len)
        : verts_0(verts_0), verts_len(verts_len), inds(inds), inds_len(inds_len),
        stride(2), loc_pos_0(1), loc_pos(0), offset_pos(0) {
    
    GLDO(glGenVertexArrays(1, &vao));
    GLDO(glGenBuffers(1, &vbo));
    GLDO(glGenBuffers(1, &vbo_0));
    GLDO(glGenBuffers(1, &ebo));
    
    GLDO(glBindVertexArray(vao));
    
    verts_size = verts_len * sizeof(float);
    GLDO(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLDO(glBufferData(GL_ARRAY_BUFFER, verts_size, verts, GL_STATIC_DRAW));
    GLDO(glVertexAttribPointer(loc_pos, stride - offset_pos,
        GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset_pos * sizeof(float))));
    GLDO(glEnableVertexAttribArray(loc_pos));
    
    GLDO(glBindBuffer(GL_ARRAY_BUFFER, vbo_0));
    GLDO(glBufferData(GL_ARRAY_BUFFER, verts_size, verts_0, GL_DYNAMIC_DRAW));
    GLDO(glVertexAttribPointer(loc_pos_0, stride - offset_pos,
        GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset_pos * sizeof(float))));
    GLDO(glEnableVertexAttribArray(loc_pos_0));
    
    GLDO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLDO(glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds_len * sizeof(unsigned int), inds, GL_STATIC_DRAW));
    
    shader_program = createShaderProgram(
        "shaders/shader.vert", "shaders/shader.frag");
}

Renderer::~Renderer() {
    GLDO(glDeleteProgram(shader_program));
}

void Renderer::Draw() {
    GLDO(glUseProgram(shader_program));
    
    GLDO(glBindVertexArray(vao));
    
    GLDO(glBufferData(GL_ARRAY_BUFFER, verts_size, verts_0, GL_DYNAMIC_DRAW));
    // std::cout << inds_len << '\n';
    GLDO(glDrawElements(GL_LINES, inds_len, GL_UNSIGNED_INT, 0));
    GLDO(glBindVertexArray(0));
}
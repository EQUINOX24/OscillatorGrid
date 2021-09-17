#include "renderer.hpp"
#include "window.hpp"
#include "mesh.hpp"
#include "vec2.hpp"

#include <iostream>
#include <cmath>

#define WIDTH 800
#define HEIGHT 800

#define LOG(x) std::cout << x << '\n'

#define K_1 2.0
#define K_3 1.9
#define DELTA_TIME 0.05

struct NodeConn {
    unsigned int self;
    unsigned int neighbor[NEIGHBOR_NODES];
};

Vec2 Force(const Vec2* pos, const NodeConn* nodes, int on) {
    Vec2 force = Vec2(0.0f, 0.0f);
    for (int n = 0; n < NEIGHBOR_NODES; n++) {
        Vec2 vec = pos[nodes[on].self] - pos[nodes[on].neighbor[n]];
        force += - K_1 * vec - K_3 * vec.Quadrance() * vec;
    }
    return force;
}

void EulerStep(Vec2* pos, Vec2* vel, const NodeConn* nodes) {
    for (int i = 0; i < ACTIVE_NODES; i++) {
        vel[nodes[i].self] += DELTA_TIME * Force(pos, nodes, i);
    }
    for (int i = 0; i < ACTIVE_NODES; i++) {
        pos[nodes[i].self] += DELTA_TIME * vel[nodes[i].self];
    }
}

int main() {
    auto window = initWindow(WIDTH, HEIGHT, "Anharmonic oscillator hexagonal grid");
    initGlad();
    if (window == NULL) { return -1; }
    glfwSwapInterval(1);
    
    float fixed_verts[VERT_ARRAY_LENGTH];
    unsigned int indices[EDGE_ARRAY_LENGTH];
    unsigned int conn_data[CONN_ARRAY_LENGTH];
    float pos_data[VERT_ARRAY_LENGTH] = {0};
    float vel_data[VERT_ARRAY_LENGTH] = {0};
    readMesh(fixed_verts, indices, conn_data);
    
    const NodeConn* nodes = (NodeConn*) conn_data;
    Vec2* pos = (Vec2*) pos_data;
    Vec2* vel = (Vec2*) vel_data;
    
    auto renderer = Renderer(
        pos_data, fixed_verts, VERT_ARRAY_LENGTH,
        indices, EDGE_ARRAY_LENGTH
    );
    
    pos[65] += 0.2 * Vec2(3, 2);
    pos[66] += 0.2 * Vec2(1, 2);
    
    while (!glfwWindowShouldClose(window)) {
        clearWindow(0.07f, 0.11f, 0.13f);
        
        EulerStep(pos, vel, nodes);
        
        renderer.Draw();
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    renderer.~Renderer();
    
    closeWindow(window);
}

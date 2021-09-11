#include "engine.hpp"
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

int main() {
    auto window = initWindow(WIDTH, HEIGHT, "Anharmonic oscillator hexagonal grid");
    if (window == NULL) { return -1; }
    glfwSwapInterval(1);
    
    
    float fixed_verts[VERT_ARRAY_LENGTH];
    unsigned int indices[EDGE_ARRAY_LENGTH];
    unsigned int conn_data[CONN_ARRAY_LENGTH];
    float pos_data[VERT_ARRAY_LENGTH] = {0};
    float vel_data[VERT_ARRAY_LENGTH] = {0};
    readMesh(fixed_verts, indices, conn_data);
    
    NodeConn* nodes = (NodeConn*) conn_data;
    Vec2* pos = (Vec2*) pos_data;
    Vec2* vel = (Vec2*) vel_data;
    
    auto renderer = Renderer(
        pos_data, fixed_verts, VERT_ARRAY_LENGTH,
        indices, EDGE_ARRAY_LENGTH
    );
    
    LOG(pos[10] << ", " << pos[11]);
    // pos[2078] += 0.08 * Vec2(1, 2);
    // pos[2079] += 0.16 * Vec2(1, 2);
    // pos[2080] += 0.16 * Vec2(1, 2);
    // pos[2081] += 0.08 * Vec2(1, 2);
    
    // pos[62] += 0.5 * Vec2(1, 2);
    pos[65] += 0.2 * Vec2(3, 2);
    pos[66] += 0.2 * Vec2(1, 2);
    
    while (!glfwWindowShouldClose(window)) {
        GLDO(glClear(GL_COLOR_BUFFER_BIT));
        GLDO(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        
        // for (int frame = 0; frame < 2; frame++) {
        for (int i = 0; i < ACTIVE_NODES; i++) {
            int m = nodes[i].self;
            Vec2 force = Vec2(0.0f, 0.0f);
            for (int n = 0; n < NEIGHBOR_NODES; n++) {
                Vec2 displacement = pos[m] - pos[nodes[i].neighbor[n]];
                force += - K_1 * displacement - K_3 * displacement.Quadrance() * displacement;
            }
            vel[m] += DELTA_TIME * force;
        }
        for (int i = 0; i < ACTIVE_NODES; i++) {
            int m = nodes[i].self;
            pos[m] += DELTA_TIME * vel[m];
        }
        // }
        
        renderer.Draw();
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    renderer.~Renderer();
    
    closeWindow(window);
}

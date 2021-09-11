import numpy as np

def list2str(L, W):
    text = ''
    if len(L) % W != 0: raise ValueError('len(L) % W != 0')
    for n in range(len(L) // W):
        text += 8 * ' '
        for w in range(W):
            text += f'{L[n*W + w]}, '
        text = text[:-1] + '\n'
    return text[:-2]

def convertToHpp(constants, vertices, edges, connections):
    return f"""#pragma once
#define ACTIVE_NODES {constants['ACTIVE_NODES']}
#define NEIGHBOR_NODES {constants['NEIGHBOR_NODES']}
#define VERT_ARRAY_LENGTH {constants['VERT_ARRAY_LENGTH']}
#define EDGE_ARRAY_LENGTH {constants['EDGE_ARRAY_LENGTH']}
#define CONN_ARRAY_LENGTH {constants['CONN_ARRAY_LENGTH']}

void readMesh(float* out_verts, unsigned int* out_edges, unsigned int* out_conns) {'{'}
    const float vertices[] = {'{'}
{vertices}
    {'}'};
    
    const unsigned int edges[] = {'{'}
{edges}
    {'}'};
    
    const unsigned int connections[] = {'{'}
{connections}
    {'}'};
    
    for (int i = 0; i < VERT_ARRAY_LENGTH; i++)
        out_verts[i] = vertices[i];
    for (int i = 0; i < EDGE_ARRAY_LENGTH; i++)
        out_edges[i] = edges[i];
    for (int i = 0; i < CONN_ARRAY_LENGTH; i++)
        out_conns[i] = connections[i];
{'}'}
"""

def SquareGrid(N, Ny):
    x = np.zeros((N,N))
    y = np.zeros((N,N))
    for n in range(N):
        x[:,n] = n / (N - 1)
        y[n,:] = n / (N - 1)
    
    x = 1.5 * x - 0.75
    y = 0.75 - 1.5 * y

    vertices = []
    for i in range(N):
        for j in range(N):
            vertices += [x[i,j], y[i,j]]

    edges = []
    for m in range((N - 1) * N):
        i = m // (N - 1)
        j = m % (N - 1)
        n_1 = N * i + j
        n_2 = N * i + (j + 1)
        edges += [n_1, n_2]

        i = m % (N - 1)
        j = m // (N - 1)
        n_1 = N * i + j
        n_2 = N * (i + 1) + j
        edges += [n_1, n_2]

    connections = []
    for n in range((N-2)**2):
        i = n // (N - 2) + 1
        j = n % (N - 2) + 1
        connections += [N*i+j, N*(i+1)+j, N*(i-1)+j, N*i+j+1, N*i+j-1]
    
    constants = {
        'ACTIVE_NODES'      : (N - 2)**2,
        'NEIGHBOR_NODES'    : 4,
        'VERT_ARRAY_LENGTH' : 2 * N**2,
        'EDGE_ARRAY_LENGTH' : 4 * N * (N - 1),
        'CONN_ARRAY_LENGTH' : 5 * (N - 2)**2
    }
    
    return convertToHpp(constants,
        list2str(vertices, 2),
        list2str(edges, 2),
        list2str(connections, 5))

def HexGrid(Nx, Ny):
    delta_y = np.sqrt(3)/2
    N_i, N_j = 2*Ny + 1, 3*Nx - 1
    
    vertices = []
    points = []
    for i in range(N_i):
        is_even = False if (i % 2) else True
        c = -1
        for j in range(N_j + (not is_even)):
            c += 1
            if c % 3 == (1 + is_even): continue
            points.append(np.array([j + 0.5*is_even, -i*delta_y]))
    
    center = sum(points)/len(points)
    vertvecs = [point - center for point in points]
    furthest = max(vertvecs, key=lambda vert: np.linalg.norm(vert))
    normdist = 7 / (8 * np.linalg.norm(furthest))
    for vert in vertvecs: vertices += (normdist * vert).tolist()
    
    edges = []
    conns = [[] for n in range(len(points))]
    for m, point_m in enumerate(points):
        for n, point_n in enumerate(points[:m]):
            if np.linalg.norm(point_m - point_n) < 1.1:
                conns[m].append(n)
                conns[n].append(m)
                edges += [m, n]
    
    connections = []
    for n, conn in enumerate(conns):
        if len(conn) == 3:
            connections += [n] + conn
    
    # connections = [0, 0, 0, 0]
    
    constants = {
        'ACTIVE_NODES'      : len(connections)//4,
        'NEIGHBOR_NODES'    : 3,
        'VERT_ARRAY_LENGTH' : len(vertices),
        'EDGE_ARRAY_LENGTH' : len(edges),
        'CONN_ARRAY_LENGTH' : len(connections)
    }
    
    return convertToHpp(constants,
        list2str(vertices, 2),
        list2str(edges, 2),
        list2str(connections, 4))
    

with open('src/mesh.hpp', 'w') as file:
    # file.write(SquareGrid(32, 32))
    file.write(HexGrid(15, 25))
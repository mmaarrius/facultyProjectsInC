// Copyright (c) 2024, Popa Marius
// All rights reserved.
#include "chunk.h"

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
        chunk[x][y][z] = block;
    }
    return chunk;
}


void reverse(int* x0, int* x1) {
    int aux = 0;
    if (*x0 > *x1) {
        aux = *x0;
        *x0 = *x1;
        *x1 = aux;
    }
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
    reverse(&x0, &x1);
    reverse(&z0, &z1);
    reverse(&y0, &y1);
    for (int z = z0; z <= z1; z++) {
        for (int y = y0; y <= y1; y++) {
            for (int x = x0; x <= x1; x++) {
                chunk = chunk_place_block(chunk, width,
                height, depth, x, y, z, block);
            }
        }
    }
    return chunk;
}

int distance(int x1, int y1, int z1, int x2, int y2, int z2) {
    return  (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (distance(i, j, k, x, y, z) <= radius * radius) {
                    chunk = chunk_place_block(chunk, width,
                    height, depth, i, j, k, block);
                }
            }
        }
    }
    return chunk;
}

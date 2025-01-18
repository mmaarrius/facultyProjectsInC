#include "chunk.h"
#include <stdlib.h>
#include <stdio.h>

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {

    int new_width = depth;
    int new_depth = width;

    // Allocate new_chunk
    char*** new_chunk = malloc(new_width * sizeof(*new_chunk));
    for (int x = 0; x < new_width; x++) {
        new_chunk[x] = malloc(height * sizeof(**new_chunk));
        for (int y = 0; y < height; y++) {
            new_chunk[x][y] = malloc(new_depth * sizeof(***new_chunk));
        }
    }

    // Spin chunk
    for (int x = 0; x < new_width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < new_depth; z++) {
                new_chunk[x][y][z] = chunk[z][y][depth - 1 - x];
            }
        }
    }

    // Free old chunk
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk[x][y]);
        }
        free(chunk[x]);
    }
    free(chunk);

    return new_chunk;
}

void fix_corps(char*** chunk, int width, int height, int depth) {
    // Coding falled elements
    int fixare = 1;
    char block = 0;
    while (fixare) {
        fixare = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int z = 0; z < depth; z++) {
                    // Fix any corp that is on the ground
                    if (y == 0 && chunk[x][y][z] > 0 && chunk[x][y][z] < 4) {
                        block = (char)(chunk[x][y][z] + 3);
                        chunk_fill(chunk, width, height, depth,
                        x, y, z, block);
                        fixare = 1;
                    }
                    // Fix any corp that is on another corp
                    if (chunk[x][y][z] > 3 && y < height - 1 && chunk[x][y + 1][z] > 0 && chunk[x][y + 1][z] < 4) {
                        block = (char)(chunk[x][y + 1][z] + 3);
                        chunk_fill(chunk, width, height, depth,
                        x, y + 1, z, block);
                        fixare = 1;
                    }
                }
            }
        }
    }
}

void is_falling(char*** chunk, int width, int height, int depth, int* exists_corps) {
    int fell = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < depth; z++) {
                // if the block is not fixed
                if (chunk[x][y][z] < 4) {
                    if (chunk[x][y][z] != 0) {
                        *exists_corps = 1;
                    }
                    // if the block is under a fixed block
                    if (y < height - 1 && chunk[x][y + 1][z] > 3) {
                        chunk[x][y][z] = 0;
                        fell = 1;
                    }
                    // if the block is on the last up layer
                    if (y == height - 1) {
                        chunk[x][y][z] = 0;
                        fell = 1;
                    }
                    // basic falling
                    if (!fell && y < height - 1) {
                        chunk[x][y][z] = chunk[x][y + 1][z];
                    }
                }
                fell = 0;
            }
        }
    }
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {
    int exists_corps = 1;

    // Gravity
    while (exists_corps) {
        exists_corps = 0;
        fix_corps(chunk, width, height, depth);
        is_falling(chunk, width, height, depth, &exists_corps);
    }

    // Decoding
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z] > 3) {
                    chunk[x][y][z] -= 3;
                }
            }
        }
    }

    // Find new height
    int free_space = 1;
    *new_height = height;
    for (int y = height - 1; y >=0; y--) {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z] != 0) {
                    free_space = 0;
                }
            }
        }
        if (free_space) {
            *new_height = *new_height - 1;
        } else {
            break;
        }
    }

    // Realloc chunk
    char** ptr = NULL;
    for (int x = 0; x < width; x++) {
        for (int y = height - 1; y >= *new_height; y--) {
            if (chunk[x][y] != NULL) {
                free(chunk[x][y]);
            }
        }
    }
    for (int x = 0; x < width; x++) {
        ptr = realloc(chunk[x], *new_height * sizeof(**chunk));
        chunk[x] = ptr;
    }

    return chunk;
}


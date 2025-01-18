// Copyright (c) 2024, Popa Marius
// All rights reserved.
#include "chunk.h"
#include <stdlib.h>

#define path -1
#define notMatchingBlock -2

char*** fix_shell(
    char*** chunk, int width, int height, int depth, int x,
    int y, int z, char target_block, char shell_block) {
    // Move back
    if (x - 1 >= 0 && chunk[x - 1][y][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x - 1, y, z, shell_block);
    // Move front
    if (x + 1 < width && chunk[x + 1][y][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x + 1, y, z, shell_block);
    // Move down
    if (y - 1 >= 0 && chunk[x][y - 1][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y - 1, z, shell_block);
    // Move up
    if (y + 1 < height && chunk[x][y + 1][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y + 1, z, shell_block);
    // Move left
    if (z - 1 >= 0 && chunk[x][y][z - 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y, z - 1, shell_block);
    // Move right
    if (z + 1 < depth && chunk[x][y][z + 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y, z + 1, shell_block);
    if (x - 1 >= 0 && y - 1 >= 0 && chunk[x - 1][y - 1][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x - 1, y - 1, z, shell_block);
    if (x - 1 >= 0 &&  y + 1 < height && chunk[x - 1][y + 1][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x - 1, y + 1, z, shell_block);
    if (x + 1 < width && y - 1 >= 0 && chunk[x + 1][y - 1][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x + 1, y - 1, z, shell_block);
    if (x + 1 < width && y + 1 < height && chunk[x + 1][y + 1][z] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x + 1, y + 1, z, shell_block);
    if (x - 1 >= 0 && z - 1 >= 0 && chunk[x - 1][y][z - 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x - 1, y, z - 1, shell_block);
    if (x - 1 >= 0 && z + 1 < depth && chunk[x - 1][y][z + 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x - 1, y, z + 1, shell_block);
    if (x + 1 < width && z - 1 >= 0 && chunk[x + 1][y][z - 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x + 1, y, z - 1, shell_block);
    if (x + 1 < width && z + 1 < depth && chunk[x + 1][y][z + 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x + 1, y, z + 1, shell_block);
    if (y - 1 >= 0 && z - 1 >= 0 && chunk[x][y - 1][z - 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y - 1, z - 1, shell_block);
    if (y - 1 >= 0 && z + 1 < depth && chunk[x][y - 1][z + 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y - 1, z + 1, shell_block);
    if (y + 1 < height && z - 1 >= 0 && chunk[x][y + 1][z - 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y + 1, z - 1, shell_block);
    if (y + 1 < height && z + 1 < depth && chunk[x][y + 1][z + 1] != target_block)
        chunk = chunk_place_block(
        chunk, width, height, depth, x, y + 1, z + 1, shell_block);

    return chunk;
}

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {

    // Allocation and initialization of chunk_copie
    char ***chunk_copie = malloc(width * sizeof(*chunk_copie));
    for (int i = 0; i < width; i++) {
        chunk_copie[i] = malloc(depth * sizeof(**chunk_copie));
        for (int j = 0; j < depth; j++) {
            chunk_copie[i][j] = calloc(height, sizeof(***chunk_copie));
        }
    }

    // Copy path in chunk_copie
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z] == target_block) {
                    chunk_copie[x][y][z] = path;
                }
            }
        }
    }

    // Fix shell in chunk
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                if (chunk_copie[x][y][z] == path) {
                    chunk = fix_shell(chunk, width, height, depth, x, y, z, target_block, shell_block);
                }
            }
        }
    }

    // Free memory
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < depth; j++) {
            free(chunk_copie[i][j]);
        }
        free(chunk_copie[i]);
    }
    free(chunk_copie);

    return chunk;
}

void fill_plan_xz(char ***chunk, char** plan2d, int width, int height, int depth,
    int x, int y, int z, char block, int block_ref) {
    plan2d[x][z] = block;

    // Moving up
    if (x - 1 >= 0 && chunk[x - 1][y][z] == block_ref && plan2d[x - 1][z] != block) {
        fill_plan_xz(chunk, plan2d, width, height, depth,
        x - 1, y, z, block, block_ref);
    }

    // Moving down
    if (x + 1 < width && chunk[x + 1][y][z] == block_ref && plan2d[x + 1][z] != block) {
        fill_plan_xz(chunk, plan2d, width, height, depth,
        x + 1, y, z, block, block_ref);
    }

    // Moving front
    if (z + 1 < depth && chunk[x][y][z + 1] == block_ref && plan2d[x][z + 1] != block) {
        fill_plan_xz(chunk, plan2d, width, height, depth,
        x, y, z + 1, block, block_ref);
    }

    // Moving back
    if (z - 1 >= 0 && chunk[x][y][z - 1] == block_ref && plan2d[x][z - 1] != block) {
        fill_plan_xz(chunk, plan2d, width, height, depth,
        x, y, z - 1, block, block_ref);
    }
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    int block_ref = chunk[x][y][z];

    // Allocation and initialization of 2D plan
    char **plan2d = malloc(width * sizeof(*plan2d));
    for (int i = 0; i < width; i++) {
        plan2d[i] = malloc(depth * sizeof(**plan2d));
        for (int j = 0; j < depth; j++) {
            plan2d[i][j] = notMatchingBlock;
        }
    }

    // Fill 2D plan
    fill_plan_xz(chunk, plan2d, width, height, depth,
    x, y, z, block, block_ref);

    // Fill chunk
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < depth; j++) {
            if (plan2d[i][j] == block) {
                chunk[i][y][j] = block;
            }
        }
    }

    // Free memory
    for (int i = 0; i < width; i++) {
        free(plan2d[i]);
    }
    free(plan2d);

    return chunk;
}

void fill_interior(char*** chunk, char*** chunk_copie, int width, int height, int depth,
    int x, int y, int z, char target_block, char block) {

    chunk[x][y][z] = block;
    chunk_copie[x][y][z] = path;

    // Move left
    if (x - 1 >= 0 && chunk_copie[x - 1][y][z] != path && chunk[x - 1][y][z] == target_block) {
        fill_interior(chunk, chunk_copie, width, height, depth,
        x - 1, y, z, target_block, block);
    }
    // Move right
    if (x + 1 < width && chunk_copie[x + 1][y][z] != path && chunk[x + 1][y][z] == target_block) {
        fill_interior(chunk, chunk_copie, width, height, depth,
        x + 1, y, z, target_block, block);
    }
    // Move front
    if (z + 1 < depth && chunk_copie[x][y][z + 1] != path && chunk[x][y][z + 1] == target_block) {
        fill_interior(chunk, chunk_copie, width, height, depth,
        x, y, z + 1, target_block, block);
    }
    // Move back
    if (z - 1 >= 0 && chunk_copie[x][y][z - 1] != path && chunk[x][y][z - 1] == target_block) {
        fill_interior(chunk, chunk_copie, width, height, depth,
        x, y, z - 1, target_block, block);
    }
    // Move up
    if (y + 1 < height && chunk_copie[x][y + 1][z] != path && chunk[x][y + 1][z] == target_block) {
        fill_interior(chunk, chunk_copie, width, height, depth,
        x, y + 1, z, target_block, block);
    }
    // Move down
    if (y - 1 >= 0 && chunk_copie[x][y - 1][z] != path && chunk[x][y - 1][z] == target_block) {
        fill_interior(chunk, chunk_copie, width, height, depth,
        x, y - 1, z, target_block, block);
    }
}

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {

    // Take target block
    char target_block = chunk[x][y][z];

    // Allocation of chunk_copie
    char ***chunk_copie = malloc(width * sizeof(*chunk_copie));
    for (int x = 0; x < width; x++) {
        chunk_copie[x] = malloc(height * sizeof(**chunk_copie));
        for (int y = 0; y < height; y++) {
            chunk_copie[x][y] = calloc(depth, sizeof(***chunk_copie));
        }
    }

    // Copy path in chunk_copie
    fill_interior(chunk, chunk_copie, width, height, depth,
    x, y, z, target_block, block);

    // Free memory
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk_copie[x][y]);
        }
        free(chunk_copie[x]);
    }
    free(chunk_copie);

    return chunk;
}

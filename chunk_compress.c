// Copyright (c) 2024, Popa Marius
// All rights reserved.
#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS_NUMBER 4095
#define BLOCK_OFFSET_BITS 6
#define TYPE_OFFSET_BITS 5
#define FULL_BYTE 0xFF
#define RUN_LENGTH_MASK_SINGLE_BYT 0b11111
#define BYTE_LENGTH 8
#define RUN_LENGTH_FIRST_PART_MASK 0b1111

unsigned char* increase_arr_len(unsigned char *arr, int length) {
    unsigned char* ptr = realloc(arr, length * sizeof(*arr));

    return ptr;
}

void setArray(unsigned char** arr, int num, char block, int *length) {
    // Set maxim bytes
    int value = num / MAX_BLOCKS_NUMBER;
    (*length) += value * 2;
    if (value != 0) *arr = increase_arr_len(*arr, *length);
    for (int i = value * 2; i > 0; i -= 2) {
        (*arr)[*length - i] = (block << BLOCK_OFFSET_BITS) +
        (1 << TYPE_OFFSET_BITS) + (FULL_BYTE >> 4);  // set the first byte
        (*arr)[*length - i + 1] = FULL_BYTE;  // set the second byte
    }

    // Set the last bytes
    num = num % MAX_BLOCKS_NUMBER;
    if (num > RUN_LENGTH_MASK_SINGLE_BYT) {
        (*length) += 2;
        *arr = increase_arr_len(*arr, *length);
        (*arr)[*length - 2] = (block << BLOCK_OFFSET_BITS) +
        (1 << TYPE_OFFSET_BITS) + (num >> BYTE_LENGTH);  // keeps the second right last byte
        (*arr)[*length - 1] = num;  // keeps the last right byte
    } else if (num > 0) {
        (*length)++;
        *arr = increase_arr_len(*arr, *length);
        (*arr)[*length - 1] = (block << BLOCK_OFFSET_BITS) + num;  // keeps the last byte
    }
}


unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {

    unsigned char* arr = NULL;

    // Crossing the chunk and working on it
    int num = 1;
    char ant = chunk[0][0][0];  // Set anterior block
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++, num++) {
                if (chunk[x][y][z] != ant) {
                    setArray(&arr, num - 1, ant, length);
                    num = 1;
                }
                if (x == width - 1 && y == height - 1 && z == depth - 1) {
                    setArray(&arr, num, chunk[x][y][z], length);
                }
                ant = chunk[x][y][z];  // Set the anterior block
            }
        }
    }

    return arr;
}

void addToChunk(
    char*** chunk, int width, int depth,
    unsigned char block, int num, int* x, int* y, int* z) {
    // Add the blocks to the chunk
    for (int i = 0; i < num; i++) {
        (*x)++;
        if (*x == width) {
            (*z)++;
            *x = 0;
            if (*z == depth) {
                (*y)++;
                *z = 0;
            }
        }
        chunk[*x][*y][*z] = (char)block;
    }
}

int findLength(unsigned char* code, int width, int height, int depth) {
    int blocks = 0, i = 0, num = 0;
    while (num != width * height * depth) {
        if ((code[i] & (1 << TYPE_OFFSET_BITS)) != 0) {
            blocks += (code[i] & RUN_LENGTH_FIRST_PART_MASK);    // get the last 4 bits
            blocks <<= BYTE_LENGTH;    // shift 8 bits to the left
            i++;    // move to the next byte
            blocks += code[i];    // get the next byte
        } else {
            blocks += (code[i] & RUN_LENGTH_MASK_SINGLE_BYT);    // get the last 5 bits
        }
        num += blocks;    // add the number of blocks to the total
        blocks = 0;
        i++;    // move to the next byte
    }

    return i;  // return the length of the code
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {
    unsigned char block = 0;
    int num = 0;

    // Chunk allocation
    char*** chunk = malloc(width * sizeof(*chunk));
    for (int x = 0; x < width; x++) {
        chunk[x] = malloc(height * sizeof(**chunk));
        for (int y = 0; y < height; y++) {
            chunk[x][y] = malloc(depth * sizeof(***chunk));
        }
    }

    // Find length of "code"
    int length = findLength(code, width, height, depth);

    // Chunk creation
    int x = -1, y = 0, z = 0;
    for (int i = 0; i < length; i++) {
        num = 0;
        block = code[i] >> BLOCK_OFFSET_BITS;    // get the first 2 bits
        if ((code[i] & (1 << TYPE_OFFSET_BITS)) != 0) {
            num += code[i] & RUN_LENGTH_FIRST_PART_MASK;    // get the last 4 bits
            num <<= BYTE_LENGTH;    // shift 8 bits to the left
            i++;    // move to the next byte
            num += code[i];    // get the next byte
        } else {
            num += code[i] & RUN_LENGTH_MASK_SINGLE_BYT;    // get the last 5 bits
        }
        addToChunk(chunk, width, depth, block, num, &x, &y, &z);
    }

    return chunk;
}


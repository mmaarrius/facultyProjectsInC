# FACULTY PROJECT

## Project Overview
This README provides an overview of my implementations for Minecraft Project - a game that all we know.

## Introduction
The `libchunk` library is designed to handle chunk-based data processing.

## File Descriptions
### `chunk_gen.c`
- **Description**: Implements the core chunk data structure and associated operations.
- **Key Functions**:
    - `chunk_place_block()`: Place an air/grass/wood/stone block in a specified position.
    - `chunk_fill_cuboid()`: Fill a cuboid with a block type ,delimitated by coordonates, just INTO the chunk.
    - `chunk_fill_sphere()`: Fill a sphere with a block type, knowing center point and radius, just INTO the chunk.

### `chunk_process.c`
- **Description**: Provides useful operations into the chunk.
- **Key Functions**:
    - `chunk_shell()`: Adds a shell to a corp.
    - `chunk_fill_xz()`: Fill a specified 2d corp with another type of block.
    - `chunk_fill()`: Fill a specified 3d corp with another type of block.

### `chunk_transform.c`
- **Description**: Provides complex functions for chunk's transform.
- **Key Functions**:
    - `chunk_rotate_y()`: Rotates a chunk with respect to y-axis.
    - `chunk_apply_gravidy()`: Simulates gravity to corps into the chunk.

## `chunh_compress.c`
- **Description**: Compress an entire chunk to the minimum memory usage (run-length encoding RLE).
- **Key Functions**:
    - `chunk_encode()`: Memorizes a chunk with a smaller memory allocation array.
    - `chunk_decode()`: Extract the chunk from array.
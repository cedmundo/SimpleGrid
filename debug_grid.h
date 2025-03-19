//
// Created by carlos on 3/18/25.
//

#ifndef SIMPLEGRID_DEBUG_GRID_H
#define SIMPLEGRID_DEBUG_GRID_H

#include "shader.h"

typedef struct {
    Shader shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} DebugGrid;

bool DebugGridLoad(DebugGrid *grid);
void DebugGridDraw(DebugGrid *grid, Camera *camera);
void DebugGridUnload(DebugGrid *grid);

#endif  // SIMPLEGRID_DEBUG_GRID_H

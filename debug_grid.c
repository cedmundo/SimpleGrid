//
// Created by carlos on 3/18/25.
//

#include "debug_grid.h"
#include <SDL3/SDL_assert.h>

static const GLfloat vertices[] = {
    1.0f,  1.0f, 0.0f,  // top right
    1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f   // top left
};

GLuint indices[] = {
    0, 1, 3,    // first triangle
    1, 2, 3     // second triangle
};

bool DebugGridLoad(DebugGrid *grid) {
    if (grid == NULL) {
        SDL_assert(grid != NULL && "DebugGridLoad: grid is NULL");
        return false;
    }

    if (!ShaderLoad(&grid->shader,
                    "assets/debug/grid.vert",
                    "assets/debug/grid.frag")) {
        return false;
    }

    glGenVertexArrays(1, &grid->vao);
    glGenBuffers(1, &grid->vbo);
    glGenBuffers(1, &grid->ebo);

    glBindVertexArray(grid->vao);
    glBindBuffer(GL_ARRAY_BUFFER, grid->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    SDL_assert(glGetError() == GL_NO_ERROR && "DebugGridLoad: error loading debug grid");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

void DebugGridDraw(DebugGrid *grid, Camera *camera) {
    ShaderUse(&grid->shader, camera);
    glBindVertexArray(grid->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_assert(glGetError() == GL_NO_ERROR && "DebugGridDraw: error drawing debug grid");
    glBindVertexArray(0);
}

void DebugGridUnload(DebugGrid *grid) {
    SDL_assert(grid != NULL && "DebugGridUnload: grid is NULL");
    ShaderUnload(&grid->shader);
}
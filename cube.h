//
// Created by carlos on 3/19/25.
//

#ifndef SIMPLEGRID_CUBE_H
#define SIMPLEGRID_CUBE_H

#include <glad/glad.h>
#include "camera.h"
#include "shader.h"
#include "transform.h"

typedef struct {
    Transform transform;
    Shader shader;
    GLuint vao;
    GLuint vbo;
} Cube;

bool CubeLoad(Cube *cube);
void CubeUpdate(Cube *cube, float delta_time);
void CubeDraw(Cube *cube, Camera *camera);
void CubeUnload(Cube *cube);

#endif  // SIMPLEGRID_CUBE_H

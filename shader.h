//
// Created by carlos on 3/15/25.
//

#ifndef SIMPLEGRID_SHADER_H
#define SIMPLEGRID_SHADER_H

#include <SDL3/SDL_stdinc.h>
#include <glad/glad.h>
#include "camera.h"

typedef struct {
    GLuint program_id;
} Shader;

bool ShaderLoad(Shader *shader, const char *vs_path, const char *fs_path);
void ShaderUse(const Shader *shader, Camera *camera);
void ShaderUnload(Shader *shader);

#endif  // SIMPLEGRID_SHADER_H

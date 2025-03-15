//
// Created by carlos on 3/15/25.
//

#ifndef SIMPLEGRID_SHADER_H
#define SIMPLEGRID_SHADER_H

#include <glad/glad.h>
#include <SDL3/SDL_stdinc.h>

typedef struct {
    GLuint program_id;
} Shader;

bool ShaderLoad(Shader *shader, const char *vs_path, const char *fs_path);
void ShaderUse(const Shader *shader);
void ShaderUnload(Shader *shader);

#endif  // SIMPLEGRID_SHADER_H

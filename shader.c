//
// Created by carlos on 3/15/25.
//

#include "shader.h"

#include <glad/glad.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_iostream.h>

bool ShaderLoad(Shader *shader, const char *vs_path, const char *fs_path) {
    bool success = false;
    SDL_IOStream *vs_stream = NULL;
    SDL_IOStream *fs_stream = NULL;
    size_t vs_size = 0;
    size_t fs_size = 0;
    GLchar *vs_source = NULL;
    GLchar *fs_source = NULL;
    GLint status = 0;
    GLchar info_log[512] = { 0 };
    GLuint vs = 0;
    GLuint fs = 0;

    if (shader == NULL || fs_path == NULL || vs_path == NULL) {
        SDL_assert(shader != NULL && "ShaderLoad: shader is NULL");
        SDL_assert(vs_path != NULL && "ShaderLoad: vs_path is NULL");
        SDL_assert(fs_path != NULL && "ShaderLoad: fs_path is NULL");
        SDL_Log("failed to load shader: %s %s, error: invalid state", vs_path, fs_path);
        goto terminate;
    }

    vs_stream = SDL_IOFromFile(vs_path, "r");
    fs_stream = SDL_IOFromFile(fs_path, "r");
    if (vs_stream == NULL || fs_stream == NULL) {
        SDL_Log("failed to load shader: %s %s, error: %s", vs_path, fs_path, SDL_GetError());
        goto terminate;
    }

    vs_size = SDL_GetIOSize(vs_stream);
    fs_size = SDL_GetIOSize(fs_stream);

    vs_source = (GLchar *)SDL_calloc(vs_size, sizeof(GLchar));
    if (vs_source == NULL) {
        SDL_Log("failed to load vertex shader: %s, error: %s", vs_path, SDL_GetError());
        goto terminate;
    }

    fs_source = (GLchar *)SDL_calloc(fs_size, sizeof(GLchar));
    if (fs_source == NULL) {
        SDL_Log("failed to load fragment shader: %s, error: %s", fs_path, SDL_GetError());
        goto terminate;
    }

    if (SDL_ReadIO(vs_stream, vs_source, vs_size) != vs_size) {
        SDL_Log("failed to load vertex shader: %s, error: %s", vs_path, SDL_GetError());
        goto terminate;
    }

    if (SDL_ReadIO(fs_stream, fs_source, fs_size) != fs_size) {
        SDL_Log("failed to load fragment shader: %s, error: %s", fs_path, SDL_GetError());
        goto terminate;
    }

    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, (const GLchar* const*)&vs_source,
                   (const GLint*)&vs_size);
    glShaderSource(fs, 1, (const GLchar* const*)&fs_source,
                   (const GLint*)&fs_size);

    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(vs, sizeof(info_log), NULL, info_log);
        SDL_Log("failed to compile vertex shader: %s, error: %s", vs_path, info_log);
        goto terminate;
    }

    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(fs, sizeof(info_log), NULL, info_log);
        SDL_Log("failed to compile fragment shader: %s, error: %s", fs_path, info_log);
        goto terminate;
    }

    shader->program_id = glCreateProgram();
    glAttachShader(shader->program_id, vs);
    glAttachShader(shader->program_id, fs);
    glLinkProgram(shader->program_id);
    glGetProgramiv(shader->program_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(shader->program_id, sizeof(info_log), NULL, info_log);
        SDL_Log("failed to link shader: %s, error: %s", vs_path, info_log);
        goto terminate;
    }

    success = true;
terminate:
    if (vs_source != NULL) {
        SDL_free(vs_source);
    }

    if (fs_source != NULL) {
        SDL_free(fs_source);
    }

    if (vs != 0) {
        glDeleteShader(vs);
    }

    if (fs != 0) {
        glDeleteShader(fs);
    }

    return success;
}

void ShaderUse(const Shader *shader, Camera *camera) {
    glUseProgram(shader->program_id);

    mat4 view = {0};
    mat4 proj = {0};
    TransformView(&camera->transform, view);
    glm_mat4_copy(camera->projection, proj);

    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "proj"),
                       1, GL_FALSE, (const GLfloat *)proj);
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "view"),
                       1, GL_FALSE, (const GLfloat *)view);
}

void ShaderUnload(Shader *shader) {
    if (shader->program_id != 0) {
        glDeleteProgram(shader->program_id);
        shader->program_id = 0;
    }
}
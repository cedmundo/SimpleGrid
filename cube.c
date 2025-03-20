//
// Created by carlos on 3/19/25.
//

#include "cube.h"

bool CubeLoad(Cube *cube) {
    // Set shader
    if(!ShaderLoad(&cube->shader, "assets/default.vert", "assets/default.frag")) {
        return false;
    }

    static const float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &cube->vao);
    glGenBuffers(1, &cube->vbo);

    glBindVertexArray(cube->vao);
    glBindBuffer(GL_ARRAY_BUFFER, cube->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Set default transformation
    TransformIdentity(&cube->transform);
    return true;
}

void CubeUpdate(Cube *cube, float delta_time) {
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 right = {1.0f, 0.0f, 0.0f};

    versor x_rot = {0};
    versor y_rot = {0};
    glm_quatv(x_rot, delta_time, right);
    glm_quatv(y_rot, delta_time, up);
    glm_quat_mul(cube->transform.rotation, x_rot, cube->transform.rotation);
    glm_quat_mul(cube->transform.rotation, y_rot, cube->transform.rotation);
}

void CubeDraw(Cube *cube, Camera *camera) {
    ShaderUse(&cube->shader, camera);

    mat4 model = {0};
    TransformModel(&cube->transform, model);

    mat4 view = {0};
    TransformView(&camera->transform, view);

    mat4 proj = {0};
    glm_mat4_copy(camera->projection, proj);

    unsigned cube_program_id = cube->shader.program_id;
    glUniformMatrix4fv(glGetUniformLocation(cube_program_id, "model"), 1, GL_FALSE, (const GLfloat *)model);
    glUniformMatrix4fv(glGetUniformLocation(cube_program_id, "view"), 1, GL_FALSE, (const GLfloat *)view);
    glUniformMatrix4fv(glGetUniformLocation(cube_program_id, "proj"), 1, GL_FALSE, (const GLfloat *)proj);
    glUniform3fv(glGetUniformLocation(cube_program_id, "viewPos"), 1, (GLfloat *)camera->transform.position);

    glBindVertexArray(cube->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CubeUnload(Cube *cube) {
    if (cube->vbo != 0) {
        glDeleteBuffers(1, &cube->vbo);
        cube->vbo = 0;
    }

    if (cube->vao != 0) {
        glDeleteVertexArrays(1, &cube->vao);
        cube->vao = 0;
    }

    ShaderUnload(&cube->shader);
}
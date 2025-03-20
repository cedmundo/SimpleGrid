//
// Created by carlos on 3/17/25.
//

#include "camera.h"
#include "cglm/quat.h"
#include "cglm/vec3.h"
#include "cglm/cam.h"

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

void CameraMakePerspective(Camera *camera, float fov, float aspect, float near, float far) {
    glm_perspective(fov, aspect, near, far, camera->projection);
    TransformIdentity(&camera->transform);

    vec3 forward = {0.0f, 0.0f, -1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_quat_for(forward, up, camera->transform.rotation);
    camera->linear_speed = 1.0f;
    camera->angular_speed = 3.0f;

    camera->transform.position[2] = 5.0f;
}

void CameraUpdate(Camera *camera, float delta_time) {
    const bool * keyboardState = SDL_GetKeyboardState(NULL);
    vec3 direction = {0};
    vec3 input_axis = {0};
    if (keyboardState[SDL_SCANCODE_W]) {
        input_axis[2] = -1.0f;
    } else if (keyboardState[SDL_SCANCODE_S]) {
        input_axis[2] = 1.0f;
    }

    if (keyboardState[SDL_SCANCODE_Q]) {
        input_axis[1] = -1.0f;
    } else if (keyboardState[SDL_SCANCODE_Z]) {
        input_axis[1] = 1.0f;
    }

    if (keyboardState[SDL_SCANCODE_A]) {
        input_axis[0] = 1.0f;
    } else if (keyboardState[SDL_SCANCODE_D]) {
        input_axis[0] = -1.0f;
    }

    // Rotate camera using pitch and yaw
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 right = {1.0f, 0.0f, 0.0f};
    vec2 mouse_delta = {0.0f, 0.0f};
    SDL_GetRelativeMouseState(&mouse_delta[0], &mouse_delta[1]);

    versor yaw_rot = {0};
    versor pitch_rot = {0};
    glm_quatv(pitch_rot, glm_rad(mouse_delta[1] * camera->angular_speed * delta_time), right);
    glm_quat_mul(camera->transform.rotation, pitch_rot, camera->transform.rotation);

    glm_quatv(yaw_rot, glm_rad(mouse_delta[0] * camera->angular_speed * delta_time), up);
    glm_quat_mul(yaw_rot, camera->transform.rotation, camera->transform.rotation);

    // Move relative to camera forward
    glm_quat_rotatev(camera->transform.rotation, input_axis, direction);
    glm_vec3_normalize(direction);
    glm_vec3_scale(direction, camera->linear_speed * delta_time, direction);
    glm_vec3_add(direction, camera->transform.position, camera->transform.position);
}

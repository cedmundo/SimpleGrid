//
// Created by carlos on 3/17/25.
//

#include "camera.h"
#include "cglm/quat.h"
#include "cglm/vec3.h"
#include "cglm/cam.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

void CameraMakePerspective(Camera *camera, float fov, float aspect, float near, float far) {
    glm_perspective(fov, aspect, near, far, camera->projection);
    TransformIdentity(&camera->transform);

    vec3 forward = {0.0f, 0.0f, -1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_quat_for(forward, up, camera->transform.rotation);
    camera->speed = 1.0f;

    camera->transform.position[2] = 5.0f;
}

void CameraUpdate(Camera *camera, float dt) {
    const bool * keyboardState = SDL_GetKeyboardState(NULL);
    vec3 direction = {0};
    vec3 input_axis = {0};
    if (keyboardState[SDL_SCANCODE_W]) {
        input_axis[2] = -1.0f;
    } else if (keyboardState[SDL_SCANCODE_S]) {
        input_axis[2] = 1.0f;
    }

    if (keyboardState[SDL_SCANCODE_Q]) {
        input_axis[1] = 1.0f;
    } else if (keyboardState[SDL_SCANCODE_E]) {
        input_axis[1] = -1.0f;
    }

    if (keyboardState[SDL_SCANCODE_A]) {
        input_axis[0] = 1.0f;
    } else if (keyboardState[SDL_SCANCODE_D]) {
        input_axis[0] = -1.0f;
    }

    glm_quat_rotatev(camera->transform.rotation, input_axis, direction);
    glm_vec3_normalize(direction);
    glm_vec3_scale(direction, camera->speed * dt, direction);
    glm_vec3_add(camera->transform.position, direction, camera->transform.position);
}

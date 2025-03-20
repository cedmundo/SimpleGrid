//
// Created by carlos on 3/18/25.
//

#include "transform.h"
#include "cglm/affine.h"

void TransformIdentity(Transform *t) {
    glm_vec3_zero(t->position);
    glm_quat_identity(t->rotation);
    glm_vec3_one(t->scale);
}

void TransformModel(Transform *t, mat4 m) {
    glm_mat4_identity(m);
    glm_translate(m, t->position);
    glm_quat_rotate(m, t->rotation, m);
    glm_scale(m, t->scale);
}

void TransformView(Transform *t, mat4 v) {
    glm_quat_look(t->position, t->rotation, v);
}
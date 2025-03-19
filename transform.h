//
// Created by carlos on 3/18/25.
//

#ifndef SIMPLEGRID_TRANSFORM_H
#define SIMPLEGRID_TRANSFORM_H

#include "cglm/quat.h"
#include "cglm/vec3.h"
#include "cglm/mat4.h"

typedef struct {
    versor rotation;
    vec3 position;
    vec3 scale;
} Transform;

void TransformIdentity(Transform *t);
void TransformModel(Transform *t, mat4 m);
void TransformView(Transform *t, mat4 v);

#endif  // SIMPLEGRID_TRANSFORM_H

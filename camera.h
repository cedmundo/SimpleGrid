//
// Created by carlos on 3/17/25.
//

#ifndef SIMPLEGRID_CAMERA_H
#define SIMPLEGRID_CAMERA_H

#include "transform.h"

typedef struct {
    Transform transform;
    mat4 projection;
    float speed;
} Camera;

void CameraMakePerspective(Camera *camera,
                           float fov, float aspect, float near, float far);
void CameraUpdate(Camera *camera, float dt);
void CameraPassUniforms(Camera *camera);


#endif  // SIMPLEGRID_CAMERA_H

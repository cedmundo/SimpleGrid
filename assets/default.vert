#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;

out vec3 normal;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    // note: costly, most of the time it will better to calculate the normal matrix when scaling once (CPU)
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    normal = normalMatrix * aNor;
    fragPosition = vec3(model * vec4(aPos, 1.0));
    mat4 mvp = proj * view * model;
    gl_Position = mvp * vec4(aPos, 1.0);
}
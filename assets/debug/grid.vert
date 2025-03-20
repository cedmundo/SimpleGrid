#version 330 core

in vec3 aPos;
out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 proj;
uniform mat4 view;
uniform vec3 viewPos;

vec3 unprojectPoint(float x, float y, float z, mat4 v, mat4 p) {
    mat4 viewInv = inverse(v);
    mat4 projInv = inverse(p);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    nearPoint = unprojectPoint(aPos.x, aPos.y, 0.01, view, proj).xyz;
    farPoint = unprojectPoint(aPos.x, aPos.y, 1.0, view, proj).xyz;
    gl_Position = vec4(aPos, 1.0);
}

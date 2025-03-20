#version 330 core

in vec3 nearPoint;
in vec3 farPoint;
out vec4 outColor;

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    outColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0));
}
#version 330 core

in vec3 normal;
in vec3 fragPosition;

uniform vec3 objectColor = vec3(0.2, 0.5, 0.2);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos = vec3(0.0, -2.0, 2.0);
uniform vec3 viewPos;
uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.5;

void main() {
    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos - fragPosition);
    float diffuseFactor = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = diffuseFactor * lightColor;

    // specular
    vec3 viewDirection = normalize(viewPos - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3 specular = specularStrength * specularFactor * lightColor;

    // ambient
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    Normal = aNormal;  // transpose(inverse(model)) 생략하고 비교해보기

    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * worldPos;
}

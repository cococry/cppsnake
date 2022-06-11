#version 460

layout(location = 0) in vec2 a_Position;

uniform mat4 u_Proj;
uniform mat4 u_Transform;

void main() {
    gl_Position = u_Proj * u_Transform * vec4(a_Position, 0.0, 1.0);
}
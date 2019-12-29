#version 430

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;
out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(vertex, 0, 1.0f);
    fragTexCoord = texCoord;
}

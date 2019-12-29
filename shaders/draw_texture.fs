#version 430

uniform sampler2D tex;
in vec2 fragTexCoord;
out vec4 color;

vec3 colors[] = {
    {0.0, 0.0, 1.0}, //Empty: Blue
    {0.0, 0.2, 0.0}, //1: Dark blue
    {1.0, 1.0, 0.0}, //2: Yellow
    {1.0, 0.5, 0.0}, //3: Orange
    {1.0, 0.0, 0.0}, //4: Red --> Illegal in final render
};

void main() {
    vec4 data = texture(tex, fragTexCoord);
    if(data.x < 5) {
        color = vec4(colors[int(data.x)], 1.0);
    } else {
        color = vec4(0.0, 0.0, 0.0, 1.0);            
    }
}
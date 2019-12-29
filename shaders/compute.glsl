#version 430
layout(local_size_x = %d, local_size_y = %d) in;
layout(r32f, binding = 0) uniform image2D img_input;
layout(r32f, binding = 1) uniform image2D img_output;

uint offset_count = 4;
ivec2 offsets[] = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1},
};

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    int heightChange = 0;
    //Check if surrounding piles topple
    for(int i=0; i<offset_count; i++) {
        vec4 data = imageLoad(img_input, coords + offsets[i]);

        //Sand from surrounding pile falls
        if(data.x >= 4)  heightChange++;
    }
    vec4 currentData = imageLoad(img_input, coords);
    //Topples if too high
    if(currentData.x >= 4)   heightChange -= 4;
    currentData.r += float(heightChange);

    imageStore(img_output, coords, currentData);
}

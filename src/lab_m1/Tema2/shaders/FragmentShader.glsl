#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
uniform int tankHP;

// Output
out vec3 world_pos;
out vec3 world_normal;
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    vec3 moddifiedColor;
    moddifiedColor = mix(color, vec3(0.80000, 1.00000, 1.00000), (3- tankHP)*0.3f);
   
    out_color = vec4(moddifiedColor, 1);
}

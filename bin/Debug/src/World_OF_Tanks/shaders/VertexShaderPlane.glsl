#version 330

// Input
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coord;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
out vec3 fragment_position;
out vec3 fragment_color;
out vec3 fragment_normal;
out vec2 fragment_texture_coord;

void main()
{
    // Pass the values to the fragment shader
    fragment_position = vertex_position;
    fragment_color = vertex_color;
    fragment_normal = vertex_normal;
    fragment_texture_coord = vertex_texture_coord;

    gl_Position =  Projection * View * Model * vec4(vertex_position, 1.0f);
}
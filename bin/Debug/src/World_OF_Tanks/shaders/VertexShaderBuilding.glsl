#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coord;
layout(location = 3) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 fragment_position;
out vec3 interpolated_color;
out vec3 fragment_normal;
out vec2 fragment_texture_coord;

void main()
{
    fragment_position = vec3(Model * vec4(vertex_position, 1.0));
    // Pass vertex color to fragment shader
    interpolated_color = vertex_color; 

    // Calculate fragment normal by transforming vertex normal
    // with the inverse-transpose of the model matrix
    fragment_normal = mat3(transpose(inverse(Model))) * vertex_normal;
    
    // Pass vertex texture coordinate to fragment shader
    fragment_texture_coord = vertex_texture_coord;

    gl_Position = Projection * View * Model * vec4(vertex_position, 1.0f);
}
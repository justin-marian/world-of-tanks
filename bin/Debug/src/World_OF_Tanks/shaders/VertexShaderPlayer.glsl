#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time;         // Animation time
uniform float Health;       // Object health
uniform vec3 ObjectColor;   // Object color

out vec3 VertexColor;

float deform(vec3 position, out float doffset_dx, out float doffset_dy)
{
    float freqX = 3.0;    // X-axis frequency
    float freqY = 4.0;    // Y-axis frequency

    float ampX = 0.2 * (1.0 - Health / 100.0); // X-axis amplitude
    float ampY = 0.15 * (1.0 - Health / 100.0); // Y-axis amplitude
    float phase = Time * 5.0;  // Animation phase

    // Calculate displacement
    float displacement = ampX * sin(position.x * freqX + phase) +
                         ampY * cos(position.y * freqY - phase);

    // Calculate derivatives for the displacement
    doffset_dx = ampX * freqX * cos(position.x * freqX + phase);
    doffset_dy = -ampY * freqY * sin(position.y * freqY - phase);

    return displacement;
}

void main()
{
    float doffset_dx, doffset_dy;
    float offset = deform(aPos, doffset_dx, doffset_dy);

    vec3 newPosition = aPos;
    newPosition.z += offset; // Apply deformation in the z-axis

    vec3 tangentX = vec3(1.0, 0.0, doffset_dx);
    vec3 tangentY = vec3(0.0, 1.0, doffset_dy);

    vec3 newNormal = normalize(cross(tangentX, tangentY));

    vec3 damageColor = vec3(1.0, 0.0, 0.0); // Red color for damage
    float healthFactor = Health / 100.0;

    // Mix object color with damage color based on health
    VertexColor = mix(damageColor, ObjectColor, healthFactor);

    gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}

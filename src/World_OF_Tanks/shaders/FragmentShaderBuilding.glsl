#version 330

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_texture_coord;

in vec3 interpolated_color;

out vec4 out_color;

// Perlin Noise Smooth Interpolation
// https://rtouti.github.io/graphics/perlin-noise-algorithm

// Generate random values
float hash(vec2 p)
{
    p = 50.0 * fract(p * 0.3183099 + vec2(0.1, 0.1));
    return fract(p.x * p.y * (p.x + p.y));
}

// Perlin noise
float noise(vec2 x)
{
    vec2 i = floor(x);
    vec2 f = fract(x);
    
    // Corners of the grid cell
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    // Smooth interpolation
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

// Map noise value to color
vec4 getColor(float randValue)
{
    if (randValue < 0.1) return vec4(0.8, 0.8, 0.8, 1.0); // Light Gray
    if (randValue < 0.2) return vec4(0.7, 0.7, 0.7, 1.0); // Slightly Darker Gray
    if (randValue < 0.3) return vec4(0.6, 0.6, 0.6, 1.0); // Medium Light Gray
    if (randValue < 0.4) return vec4(0.5, 0.5, 0.5, 1.0); // Neutral Gray
    if (randValue < 0.5) return vec4(0.4, 0.4, 0.4, 1.0); // Medium Dark Gray
    if (randValue < 0.6) return vec4(0.3, 0.3, 0.3, 1.0); // Darker Gray
    if (randValue < 0.7) return vec4(0.2, 0.2, 0.2, 1.0); // Very Dark Gray
    if (randValue < 0.8) return vec4(0.1, 0.1, 0.1, 1.0); // Almost Black
    return vec4(0.05, 0.05, 0.05, 1.0);                   // Nearly Black (Default)
}

void main()
{
    float n = noise(fragment_position.xy);   // Perlin noise value
    vec4 color = getColor(n);                // Map noise value to color
    out_color = color;                       // Set the output fragment color
}
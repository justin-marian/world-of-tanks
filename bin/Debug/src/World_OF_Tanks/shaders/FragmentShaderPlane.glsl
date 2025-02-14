#version 330

in vec2 fragment_texture_coord;
uniform float Time;
out vec4 out_color; 

// Perlin Noise Smooth Interpolation With Fractal Brownian Motion
// https://www.shadertoy.com/view/lltcWl
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
    
    // Four corners of the tile
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    // Smooth Interpolation
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

// Perlin noise
float layeredNoise(vec2 position, float time)
{
    int layers = 18;
    float n = 0.0;
    float amplitude = 1.0;
    float totalAmplitude = 0.0;
    float frequency = 1.0;

    // Parameters for non-periodic wave
    float waveSpeed = 1.0;  
    // Frequency of the base wave
    float waveFrequency = 2.0;
    // How much the noise affects the wave
    float noiseInfluence = 0.5;

    for (int i = 0; i < layers; i++)
    {
        // Base wave component
        float wave = sin(position.x * waveFrequency + time * waveSpeed);

        // Noise-based distortion
        float noiseDistortion = noise(position * frequency) * noiseInfluence;
        
        // Combine base wave with noise distortion
        float combinedWave = wave + noiseDistortion;
        
        // Apply the combined wave to the noise function
        n += noise(position * frequency + vec2(combinedWave, combinedWave)) * amplitude;
        
        // Amplitude function, influence height wave
        totalAmplitude += amplitude;
        
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return n / totalAmplitude;
}

// Map noise value to color
vec4 getGrayColor(float randValue)
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

// Get a random brown color
vec4 getBrownColor(float randValue)
{
    vec4 lightBrown = vec4(0.82, 0.70, 0.55, 1.0);
    vec4 mediumBrown = vec4(0.65, 0.50, 0.39, 1.0);
    vec4 darkBrown = vec4(0.36, 0.25, 0.20, 1.0);

    // Return a brown color based on the input value
    if (randValue < 0.33) return lightBrown;
    if (randValue < 0.66) return mediumBrown;
    return darkBrown;
}

// Crater-like circle with gradient
float craterCircle(vec2 uv, vec2 position, float radius)
{
    float dist = length(uv - position) / radius;
    return smoothstep(1.0, 0.8, dist); // Gradient edge
}

// Create a color gradient inside the circle
vec4 getCraterColor(float randValue, float dist)
{
    vec4 centerColor = vec4(0.36, 0.25, 0.20, 1.0); // Dark Brown (Center)
    vec4 edgeColor = vec4(0.82, 0.70, 0.55, 1.0);   // Light Brown (Edge)
    return mix(centerColor, edgeColor, dist);       // Interpolate based on distance from center
}

void main()
{
    // Adjust the scale for more or fewer spots
    float scale = 10.0;
    // Generate noise based on fragment position
    float randValue = layeredNoise(fragment_texture_coord * scale, Time);

    // Base color from Perlin noise
    vec4 baseColor = getGrayColor(randValue);

    // Heat histogram effect with static crater circles
    vec4 craterColor = vec4(0.0);
    float craterEffect = 0.0;

    // Define the number of craters along each axis
    int numCratersX = 3;
    int numCratersY = 3;

    for (int i = 0; i < numCratersX; ++i)
    {
        for (int j = 0; j < numCratersY; ++j)
        {
            float x = (float(i) + 0.5) / float(numCratersX);
            float y = (float(j) + 0.5) / float(numCratersY);

            // Add random offset to each grid position
            float offsetX = 0.05 * (fract(sin(float(i) * 12.345) * 54321.1234) - 0.5);
            float offsetY = 0.05 * (fract(cos(float(j) * 67.890) * 12345.6789) - 0.5);

            vec2 circlePos = vec2(x + offsetX, y + offsetY);
            float randRadius = 0.05 + 0.05 * fract(sin(float(i * numCratersY + j) * 12.345) * 54321.1234);

            float dist = length(fragment_texture_coord - circlePos) / randRadius;
            craterEffect += craterCircle(fragment_texture_coord, circlePos, randRadius);
            craterColor += getCraterColor(randValue, dist) * craterCircle(fragment_texture_coord, circlePos, randRadius);
        }
    }

    // Blend the crater effect with the base color
    out_color = mix(baseColor, craterColor, craterEffect);
}

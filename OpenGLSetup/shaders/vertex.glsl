#version 330 core

// Input vertex position from OpenGL application
layout(location = 0) in vec3 aPos;

// Output UV coordinates to the fragment shader
out vec2 fragCoord;

uniform vec2 iResolution; // Resolution of the viewport
//uniform mat4 model; // Using matrix for transformations of rectangles

void main() {
    // Pass the normalized device coordinates to the fragment shader
    fragCoord = (aPos.xy + 1.0) / 2.0 * iResolution;

    // Transform the vertex position into clip space
    gl_Position = vec4(aPos, 1.0);
}

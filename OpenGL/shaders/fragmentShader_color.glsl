#version 330 core

in vec3 vColor; // Matches the `out` variable from Vertex Shader
layout(location=0) out vec4 out_FragmentColor;

void main(void) {
    // Each pixel gets a blended color depending on its location
    out_FragmentColor = vec4(vColor, 1.0);
}
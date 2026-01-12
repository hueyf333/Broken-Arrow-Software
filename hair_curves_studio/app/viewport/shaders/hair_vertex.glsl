#version 330

// Vertex shader for hair curve rendering

in vec3 in_position;
in float in_width;

out float v_width;

uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    v_width = in_width;
    gl_Position = u_projection * u_view * vec4(in_position, 1.0);
}

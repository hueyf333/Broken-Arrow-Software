#version 330

// Fragment shader for hair curve rendering

in float v_width;

out vec4 fragColor;

uniform vec3 u_hair_color;

void main() {
    fragColor = vec4(u_hair_color, 1.0);
}

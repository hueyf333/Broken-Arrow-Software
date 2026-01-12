#version 330

// Vertex shader for mesh rendering

in vec3 in_position;
in vec3 in_normal;
in vec2 in_uv;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    vec4 world_pos = u_model * vec4(in_position, 1.0);
    v_position = world_pos.xyz;
    v_normal = mat3(u_model) * in_normal;
    v_uv = in_uv;
    
    gl_Position = u_projection * u_view * world_pos;
}

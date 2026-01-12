#version 330

// Fragment shader for mesh rendering

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 fragColor;

uniform vec3 u_light_dir;
uniform vec3 u_light_color;
uniform vec3 u_base_color;
uniform float u_ambient;
uniform bool u_wireframe;

void main() {
    if (u_wireframe) {
        fragColor = vec4(u_base_color, 1.0);
        return;
    }
    
    vec3 normal = normalize(v_normal);
    vec3 light = normalize(u_light_dir);
    
    // Diffuse lighting
    float diff = max(dot(normal, light), 0.0);
    vec3 diffuse = diff * u_light_color;
    
    // Ambient
    vec3 ambient = u_ambient * u_light_color;
    
    // Final color
    vec3 color = (ambient + diffuse) * u_base_color;
    fragColor = vec4(color, 1.0);
}

#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex;
layout(location = 2) in vec3 a_nor;

out vec2 v_tex;
out vec3 v_nor;

uniform mat4 u_cam;
uniform mat4 u_mat;

void main() {
	gl_Position = u_cam * u_mat * vec4(a_pos, 1.0);
	v_tex = a_tex;
	v_tex.y = 1.0 - v_tex.y;
	v_nor = (u_mat * vec4(a_nor, 1.0)).xyz - (u_mat * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
}
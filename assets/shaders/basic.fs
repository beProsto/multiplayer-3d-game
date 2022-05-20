#version 330 core
in vec2 v_tex;
in vec3 v_nor;

layout(location = 0) out vec4 col;

uniform sampler2D u_texid;

uniform vec3 u_lightcolor;
uniform float u_lightambient;
uniform vec3 u_lightdir;

void main() {
	vec3 res = texture(u_texid, v_tex).rgb;
	
	vec3 ambient = u_lightcolor * u_lightambient;

	vec3 diffuse = u_lightcolor * max(dot(normalize(-u_lightdir), normalize(v_nor)), 0.0); 

	res *= ambient + diffuse / 2.0;
	col = vec4(res, 1.0);
}
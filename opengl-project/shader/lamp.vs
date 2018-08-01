#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out VS_OUT {
	vec3 viewPos;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.viewPos = viewPos;

}
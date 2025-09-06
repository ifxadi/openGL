#version 410 core

in vec3 v_vertexColor;
out vec4 color;

uniform float uOffset;

void main()
{
	color = vec4(v_vertexColor.r, v_vertexColor.g + uOffset, v_vertexColor.b, 0.0f);
}
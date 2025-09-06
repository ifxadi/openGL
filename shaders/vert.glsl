#version 410 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColor;

uniform float uOffset;

out vec3 v_vertexColor;

void main()
{
    v_vertexColor = vertexColor;
    gl_Position = vec4(position.x, position.y + uOffset, position.z, 1.0f);
}
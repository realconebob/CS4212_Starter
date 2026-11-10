#version 330 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void)
{
  vColor = in_Color;
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}
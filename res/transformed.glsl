#shader vertex
#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_textureCoord;

out vec2 TexCoord;

uniform mat4 u_model;

void main()
{
  gl_Position = u_model  * vec4(a_pos, 0.0, 1.0);
  TexCoord = a_textureCoord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_texture;

void main()
{
  FragColor = texture(u_texture, TexCoord);
}

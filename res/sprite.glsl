#shader vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texture_coord;

out vec2 TexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
  gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
  TexCoord = a_texture_coord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_texture;

void main()
{
  vec4 texColor = texture(u_texture, TexCoord);
  FragColor = texColor;
}

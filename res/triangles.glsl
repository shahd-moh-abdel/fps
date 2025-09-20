#shader vertex
#version 330 core

layout (location = 0 ) in vec2 position;
layout (location = 1 ) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
  gl_Position = vec4(position, 0.0, 1.0);
  TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
out vec4 fragColor;

uniform sampler2D u_wallTexture;

void main()
{
  fragColor = texture(u_wallTexture, TexCoord);
}

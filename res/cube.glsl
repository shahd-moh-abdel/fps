#shader vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texture_coord;
layout (location = 2) in vec3 a_normal;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
  FragPos = vec3(u_model * vec4(a_pos, 1.0));

  Normal = mat3(transpose(inverse(u_model))) * a_normal;
  
  gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);

  TexCoord = a_texture_coord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D u_texture;
uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_viewPos;

void main()
{
  vec3 textureColor = texture(u_texture, TexCoord).rgb;

  vec3 norm = normalize(Normal);

  float ambientStrength = 0.55;
  vec3 ambient = ambientStrength * u_lightColor;

  vec3 lightDir = normalize(u_lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * u_lightColor;

  float specularStrength = 0.3;
  vec3 viewDir = normalize(u_viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * u_lightColor;

  float distance = length(u_lightPos - FragPos);
  float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

  diffuse *= attenuation;
  specular *= attenuation;

  vec3 result = (ambient + diffuse + specular) * textureColor;
  
  FragColor = vec4(result, 1.0);
}

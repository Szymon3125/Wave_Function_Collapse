#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 v_Position;

void main() {
	v_Position = (u_Transform * vec4(a_Position, 1.0)).xyz;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 _color;

uniform sampler2D u_TextureDiffuse1;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform vec3 u_AmbientColor;

in vec2 v_TexCoord;
in vec3 v_Position;

void main() {
	float c = sqrt(v_Position.y) / 2;
	_color = vec4(c, c, c, 1.0);
	//_color = texture(u_TextureDiffuse1, v_TexCoord);
}

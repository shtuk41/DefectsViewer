#version 330
uniform int		  mapMode;	

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;

in vec2 vTexture;
out vec2 vTexCoord;


uniform mat4 model_view;
uniform mat4 projection;

out vec3 position;
out vec3 vN;
out vec4 SpecularColor;
out float SpecularExponent;

smooth out vec4 theColor;

void main()
{
	vec4 vNormal = vec4(vNormal,0);
	vec4 veyepos = model_view * vPosition;
	SpecularColor		=	vec4(1,1,1,1);
	SpecularExponent	=	100;

	vN					= normalize(model_view * vNormal).xyz;

	position			= veyepos.xyz;

	gl_Position =	projection * model_view * vPosition;
	vTexCoord	= 	vTexture;
	theColor	=	vColor;


}
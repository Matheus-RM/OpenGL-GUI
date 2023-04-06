#version 330 core

// the position need to be float, so opengl can interpolate it to the fragment shader (in fPosition)

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;

uniform mat4 orthoViewMat;
uniform mat4 modelMat;
uniform float priority;

out vec2 fPosition;
out vec4 fColor;

void main()
{
	fPosition = vPosition.xy;
	fColor = vColor;

	gl_Position = orthoViewMat * modelMat* vec4(vPosition.xy, priority, 1.0); 
}
 
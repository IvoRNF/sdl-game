// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// This should correspond to the data stored
// for each vertex in the vertex buffer.
// For now, just a position.


uniform mat4 transformMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inColor;
// Add texture coordinate as output
out vec2 fragTexCoord;
out vec4 fragColor;
void main()
{
	// The vertex shader needs to output a 4D
	// coordinate.
	// For now set the 4th coordinate to 1.0
	vec4 pos = transformMatrix * vec4(inPosition,1) ;
    vec4 newPos = pos * viewMatrix *  projectionMatrix;

	gl_Position = /*transformMatrix **/ newPos; 
	////gl_Position = pos * projectionMatrix * transformMatrix;
	//gl_Position = transformMatrix * pos;
	fragTexCoord = inTexCoord;
	fragColor = inColor;
}

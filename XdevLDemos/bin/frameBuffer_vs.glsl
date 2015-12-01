layout(location = 0) in vec3 iposition;
layout(location = 9) in vec2 itexcoord;

uniform mat4 projMatrix;

out vec2 textureCoordinate;

void main()
{
	textureCoordinate = itexcoord;
	
	gl_Position = projMatrix*vec4(iposition.x, iposition.y, 0.0f, 1.0f);
}
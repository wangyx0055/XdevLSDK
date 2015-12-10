layout(location = 0) in vec3 iposition;
layout(location = 1) in vec4 icolor;
layout(location = 2) in vec3 inormal;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;

out vec4 color;
out vec3 normal;

void main() {
	
	color = icolor;
		
	normal = normalize(modelMatrix*vec4(inormal, 0.0)).xyz;
		
    gl_Position = projViewMatrix * modelMatrix * vec4(iposition, 1.0);
}

in vec4 color;			// We've got the interpolated.
in vec3 normal;			// We've got the interpolated normal.

vec3 lightDir = vec3(0.0, 0.0, -1.0);

layout(location = 0) out vec4 ocolor;

void main(void) {
	
		// Super easy lighting.
		float fs = max(0.0, dot(normal, -lightDir));
		
    ocolor = fs*color;
		
}
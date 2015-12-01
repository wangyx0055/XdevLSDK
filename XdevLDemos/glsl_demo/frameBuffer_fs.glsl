in vec2 textureCoordinate;

uniform sampler2D texture0;

layout(location = 0) out vec4 ocolor;
 
void main(void) {

    ocolor = texture(texture0, textureCoordinate);
		
}
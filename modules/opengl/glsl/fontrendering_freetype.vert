layout(location = 0) in vec4 coord;

out vec2 texCoord;

void main(void) {
	gl_Position = vec4(coord.xy, 0, 1);
	texCoord = coord.zw;
}
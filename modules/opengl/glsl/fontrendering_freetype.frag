uniform sampler2D texture;
uniform vec4 color;

in vec2 texCoord;

void main(void) {
	gl_FragColor = vec4(1.0f,1.0f,1.0f, texture2D(texture, texCoord).a) * color;
	gl_FragDepth = 0.0;
}
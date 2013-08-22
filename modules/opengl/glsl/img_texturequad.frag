uniform sampler2D tex_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_;
    gl_FragColor = texture2D(tex_, texCoords);
}
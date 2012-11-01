uniform sampler2D colorTex_;
uniform vec2 dimension_;
void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_; //TODO: replace vec2(512.0) by screenDimRCP
    gl_FragColor = texture2D(colorTex_, texCoords);
    gl_FragDepth = 0.0;
}
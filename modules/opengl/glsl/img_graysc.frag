uniform sampler2D inport_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_; //TODO: replace vec2(512.0) by screenDimRCP
    vec3 inputColor = texture2D(inport_, texCoords).rgb;
    float gray = 0.2989*inputColor.r + 0.5870*inputColor.g + 0.1140*inputColor.b;
    vec4 color = vec4(gray, gray, gray, 1.0);
    gl_FragColor = color;
}
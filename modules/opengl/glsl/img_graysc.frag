uniform sampler2D inport0_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_; //TODO: replace vec2(512.0) by screenDimRCP
    
    vec3 color0 = texture2D(inport0_, texCoords).rgb;
    float gray = 0.2989*color0.r + 0.5870*color0.g + 0.1140*color0.b;
    vec4 color = vec4(gray, gray, gray, 1.0);
    gl_FragColor = color;
    gl_FragDepth = 0.0;
}
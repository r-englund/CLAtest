uniform sampler2D inport0_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_;
    
    vec3 color0 = texture2D(inport0_, texCoords).rgb;
    gl_FragColor = vec4(color0.rgb,1);
    gl_FragDepth = 1.0;
}
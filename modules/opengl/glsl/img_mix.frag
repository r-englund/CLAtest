uniform sampler2D inport0_;
uniform sampler2D inport1_;
uniform float alpha_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_; //TODO: replace vec2(512.0) by screenDimRCP
    vec3 color0 = texture2D(inport0_, texCoords).rgb;
    vec3 color1 = texture2D(inport1_, texCoords).rgb;
    vec4 color = vec4(mix(color0, color1, alpha_), 1.0);
    gl_FragColor = color;
    gl_FragDepth = 0.0;
}
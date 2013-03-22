uniform float value_;
uniform vec2 dimension_;

void main() { 
    vec4 color = vec4(vec3(value_), 1.0f);
    gl_FragColor = color;
    gl_FragDepth = 0.0;
}
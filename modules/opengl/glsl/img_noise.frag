uniform vec2 dimension_;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_;
    gl_FragColor = vec4(vec3(rand(texCoords)), 1.0);
    gl_FragDepth = 0.0;
}
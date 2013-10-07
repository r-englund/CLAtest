uniform sampler2D color_;
//uniform sampler2D depth_;
//uniform sampler2D picking_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_;
    FragData0 = texture2D(color_, texCoords);
    //PickingData = texture2D(picking_, texCoords);
    //gl_FragDepth = texture2D(depth_, texCoords);
}
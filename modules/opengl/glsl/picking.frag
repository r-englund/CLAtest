uniform vec3 pickingColor_;

void main() {
    FragData0 = gl_Color;
    PickingData = vec4(pickingColor_, 1.0);
}
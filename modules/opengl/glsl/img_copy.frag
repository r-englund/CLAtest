uniform sampler2D color_;
uniform sampler2D depth_;
uniform sampler2D picking_;

void main() {
    FragData0 = texture2D(color_, gl_TexCoord[0].st);
    PickingData = texture2D(picking_, gl_TexCoord[0].st);
    gl_FragDepth = texture2D(depth_, gl_TexCoord[0].st).r;
}
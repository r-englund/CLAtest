uniform sampler2D tex_;

void main() {
    FragData0 = texture2D(tex_, gl_TexCoord[0].st);
}
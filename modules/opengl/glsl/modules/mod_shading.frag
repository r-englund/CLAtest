uniform vec3 lightPosition_;

uniform vec3 lightColorAmbient_;
uniform vec3 lightColorDiffuse_;
uniform vec3 lightColorSpecular_;
uniform int lightSpecularExponent_;

vec3 shadeAmbient(vec3 colorAmb) {
    return colorAmb * lightColorAmbient_;
}

vec3 shadeDiffuse(vec3 colorDiff, vec3 gradient, vec3 lightPos) {
    return colorDiff * lightColorDiffuse_ * max(dot(gradient,lightPos), 0.0);
}

vec3 shadeSpecular(vec3 colorSpec, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 halfway = normalize(cameraPos + lightPos);
    return colorSpec * lightColorSpecular_ * pow(max(dot(gradient, halfway), 0.0), lightSpecularExponent_);
}
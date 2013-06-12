vec3 shadeAmbient(vec3 colorAmb, vec3 ka) {
    return colorAmb * ka;
}

vec3 shadeDiffuse(vec3 colorDiff, vec3 kd, vec3 gradient, vec3 lightPos) {
    return colorDiff * kd * max(dot(gradient,lightPos), 0.0);
}

vec3 shadeSpecular(vec3 colorSpec, vec3 ks, float alpha, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 halfway = normalize(cameraPos + lightPos);
    return colorSpec * ks * pow(max(dot(gradient, halfway), 0.0), alpha);
}
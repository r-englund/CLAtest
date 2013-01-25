vec4 gradientForwardDiff(float intensity, sampler3D volume, vec3 samplePos) {
    vec3 delta = vec3(0.005);
    float nX = getVoxel(volume, samplePos+vec3(delta.x, 0.0, 0.0)).a;
    float nY = getVoxel(volume, samplePos+vec3(0.0, delta.y, 0.0)).a;
    float nZ = getVoxel(volume, samplePos+vec3(0.0, 0.0, delta.z)).a;
    return vec4(intensity-nX, intensity-nY, intensity-nZ, intensity);
}

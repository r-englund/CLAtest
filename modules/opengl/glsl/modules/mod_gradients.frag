vec3 gradientForwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    vec3 delta = vec3(0.005);
    float nX = getVoxel(volume, volumeParams, samplePos+vec3(delta.x, 0.0, 0.0)).a;
    float nY = getVoxel(volume, volumeParams, samplePos+vec3(0.0, delta.y, 0.0)).a;
    float nZ = getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, delta.z)).a;
    return vec3(intensity-nX, intensity-nY, intensity-nZ);
}

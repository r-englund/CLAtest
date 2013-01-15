vec4 getVoxel(sampler3D volume, vec3 samplePos) {
    return texture3D(volume, samplePos);
}

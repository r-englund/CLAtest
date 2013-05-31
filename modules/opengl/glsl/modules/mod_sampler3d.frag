struct VOLUME_STRUCT {
    vec2 dimensions_;
    vec2 dimensionsRCP_;

    sampler3D volume_;
};

vec4 getVoxel(sampler3D volume, vec3 samplePos) {
    return texture3D(volume, samplePos);
}

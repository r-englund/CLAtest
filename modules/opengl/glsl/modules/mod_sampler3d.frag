#define VOLUME_TYPE sampler3D

struct VOLUME_PARAMETERS {
    vec3 dimensions_;
    vec3 dimensionsRCP_;
};

vec4 getVoxel(VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    return texture3D(volume, samplePos);
}
vec4 applyTF(sampler2D transferFunction, vec4 voxel) {
    return texture2D(transferFunction, vec2(voxel.a, 0.5));
}

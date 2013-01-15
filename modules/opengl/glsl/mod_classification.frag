vec4 applyTF(vec4 voxel) {
    if (voxel.a < 0.05) return vec4(0.0);
    return vec4(1.0, 1.0, 1.0, 1.0)*voxel.a*5.0;
}

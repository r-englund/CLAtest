vec4 applyTF(vec4 voxel) {
    return vec4(vec3(voxel.a*50.0), voxel.a*10.0);
}

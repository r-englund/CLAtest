vec4 applyTF(sampler2D transferFunction, vec4 voxel) {
    //return vec4(vec3(voxel.a*50.0), voxel.a*10.0);
    return 10.0*texture2D(transferFunction, vec2(voxel.a, 0.5));
}

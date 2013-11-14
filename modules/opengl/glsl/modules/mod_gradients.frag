vec3 gradientForwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;
    // Value at f(x+h)
    vec3 fDs;
    fDs.x = getVoxel(volume, volumeParams, samplePos+vec3(voxelSpacing.x, 0.0, 0.0)).a;
    fDs.y = getVoxel(volume, volumeParams, samplePos+vec3(0.0, voxelSpacing.y, 0.0)).a;
    fDs.z = getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, voxelSpacing.z)).a;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( f(x+h)-f(x) ) / voxelSpacing
    return (fDs-intensity)/(voxelSpacing);
}

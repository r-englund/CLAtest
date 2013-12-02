/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

vec3 gradientForwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^2) forward differences
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

vec3 gradientCentralDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {    
	//Of order O(h^2) central differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;    
    vec3 cDs;	
	// Value at f(x+h)
    cDs.x = getVoxel(volume, volumeParams, samplePos+vec3(voxelSpacing.x, 0.0, 0.0)).a;
    cDs.y = getVoxel(volume, volumeParams, samplePos+vec3(0.0, voxelSpacing.y, 0.0)).a;
    cDs.z = getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, voxelSpacing.z)).a;
	// Value at f(x-h)
	cDs.x = cDs.x - getVoxel(volume, volumeParams, samplePos-vec3(voxelSpacing.x, 0.0, 0.0)).a;
    cDs.y = cDs.y - getVoxel(volume, volumeParams, samplePos-vec3(0.0, voxelSpacing.y, 0.0)).a;
    cDs.z = cDs.z - getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, voxelSpacing.z)).a;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( f(x+h)-f(x-h) ) / 2*voxelSpacing
    return (cDs)/(2.0*voxelSpacing);
}

vec3 gradientBackwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^2) backward differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;
    // Value at f(x-h)
    vec3 fDs;
    fDs.x = getVoxel(volume, volumeParams, samplePos-vec3(voxelSpacing.x, 0.0, 0.0)).a;
    fDs.y = getVoxel(volume, volumeParams, samplePos-vec3(0.0, voxelSpacing.y, 0.0)).a;
    fDs.z = getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, voxelSpacing.z)).a;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( f(x)-f(x-h) ) / voxelSpacing
    return (intensity-fDs)/(voxelSpacing);
}

//Higher order gradients

vec3 gradientCentralDiffH(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {    
	//Of order O(h^4) central differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;    
    vec3 cDs;
	// f' = ( -f(x+2h)+8.f(x+h)-8.f(x-h)+f(x-2h) ) / 12*h
	// Value at 8.f(x+h)
    cDs.x = 8.0 * getVoxel(volume, volumeParams, samplePos+vec3(voxelSpacing.x, 0.0, 0.0)).a;
    cDs.y = 8.0 * getVoxel(volume, volumeParams, samplePos+vec3(0.0, voxelSpacing.y, 0.0)).a;
    cDs.z = 8.0 * getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, voxelSpacing.z)).a;	
	// Value at 8.f(x-h)
	cDs.x = cDs.x - 8.0 * getVoxel(volume, volumeParams, samplePos-vec3(voxelSpacing.x, 0.0, 0.0)).a;
    cDs.y = cDs.y - 8.0 * getVoxel(volume, volumeParams, samplePos-vec3(0.0, voxelSpacing.y, 0.0)).a;
    cDs.z = cDs.z - 8.0 * getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, voxelSpacing.z)).a;
	// Value at -f(x+2h)
	cDs.x = cDs.x - getVoxel(volume, volumeParams, samplePos+vec3(2.0*voxelSpacing.x, 0.0, 0.0)).a;
    cDs.y = cDs.y - getVoxel(volume, volumeParams, samplePos+vec3(0.0, 2.0*voxelSpacing.y, 0.0)).a;
    cDs.z = cDs.z - getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, 2.0*voxelSpacing.z)).a;
	// Value at f(x+2h)
	cDs.x = cDs.x + getVoxel(volume, volumeParams, samplePos-vec3(2.0*voxelSpacing.x, 0.0, 0.0)).a;
    cDs.y = cDs.y + getVoxel(volume, volumeParams, samplePos-vec3(0.0, 2.0*voxelSpacing.y, 0.0)).a;
    cDs.z = cDs.z + getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, 2.0*voxelSpacing.z)).a;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( -f(x+2h)+8.f(x+h)-8.f(x-h)+f(x-2h) ) / 12*voxelSpacing
    return (cDs)/(12.0*voxelSpacing);
}

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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef GRADIENTS_CL
#define GRADIENTS_CL

#include "samplers.cl"

/**
 * Calculate unnormalized gradient using central difference.
 * Computation is performed and returned in texture space.
 * 
 * @param voxelSpacing The distance between voxels in each dimension.
 * @return Unnormalized gradient.
 */

float4 calcGradient(float4 pos, read_only image3d_t volume, float4 voxelSpacing) {
    float4 gradient;
    gradient.x = getVoxel(volume, smpNormClampEdgeLinear, (float4)(pos.x+voxelSpacing.x, pos.yzw)) - getVoxel(volume, smpNormClampEdgeLinear, (float4)(pos.x-voxelSpacing.x, pos.yzw));
    gradient.y = getVoxel(volume, smpNormClampEdgeLinear, (float4)(pos.x, pos.y+voxelSpacing.y, pos.zw)) - getVoxel(volume, smpNormClampEdgeLinear, (float4)(pos.x, pos.y-voxelSpacing.y, pos.zw));
    gradient.z = getVoxel(volume, smpNormClampEdgeLinear, (float4)(pos.x, pos.y, pos.z+voxelSpacing.z, pos.w)) - getVoxel(volume, smpNormClampEdgeLinear, (float4)(pos.x, pos.y, pos.z-voxelSpacing.z, pos.w));
    gradient.w = 0.f;
    gradient.xyz /= (2.f*voxelSpacing.xyz);
    return gradient;
}


#endif 
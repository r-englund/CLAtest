#ifndef SAMPLERS_CL
#define SAMPLERS_CL

__constant sampler_t smpUNormNoClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
__constant sampler_t smpUNormClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
__constant sampler_t smpNormClampLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;
__constant sampler_t smpNormClampEdgeLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;


float getVoxel(read_only image3d_t volume, sampler_t sampler, float4 pos) {
    return read_imagef(volume, sampler, pos).x;  
}

#endif // SAMPLERS_CL

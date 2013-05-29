
//#include "samplers.cl"
__constant sampler_t smpUNormNoClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;

__kernel void grayscaleKernel(read_only image2d_t src, write_only image2d_t dst) 
{ 
    if (get_global_id(0) >= get_image_width(dst) || get_global_id(1) >= get_image_height(dst)) { return; } 
	
	float4 color = read_imagef(src, smpUNormNoClampNearest, (int2)(get_global_id(0), get_global_id(1)));
    float gray = 0.2989*color.x + 0.5870*color.y + 0.1140*color.z;
    float4 result = (float4)(gray, gray, gray, color.w);
   
    write_imagef(dst, (int2)(get_global_id(0), get_global_id(1)), result);
};
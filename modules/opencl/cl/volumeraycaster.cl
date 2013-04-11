

__constant sampler_t smpUNormNoClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;

__kernel void raycaster(//read_only image3d_t volume
                        // read_only image2d_t entryPoints
                        //, read_only image2d_t exitPoints
                        //, float samplingRate
                         write_only image2d_t output) 
{
    int2 globalId = (int2)(get_global_id(0), get_global_id(1)); 
    //if( any(globalId >= get_image_dim(output)) ) {
    //    return;
    //}
    //float4 result = read_imagef(entryPoints, smpUNormNoClampNearest, globalId); 
    float4 result = (float4)(0.f, 0.f, 0.f, 0.f);
    write_imagef(output, globalId,  result);    
}




__constant sampler_t smpUNormNoClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
__constant sampler_t smpNormClampLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;
__constant sampler_t smpNormClampEdgeLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;

__constant float REF_SAMPLING_INTERVAL = 150.f;
#define ERT_THRESHOLD 1.0

__kernel void raycaster(read_only image3d_t volume
                        , read_only image2d_t entryPoints
                        , read_only image2d_t exitPoints
                        , read_only image2d_t transferFunction 
                        , float stepSize
                        , uint3 volumeDimension
                        , write_only image2d_t output) 
{
    int2 globalId = (int2)(get_global_id(0), get_global_id(1));      
    //uint3 volumeDimension = (uint3)(128,128, 128); 
    //if( any(globalId >= get_image_dim(output)) ) {
    //    return;
    //}
    float4 entry = read_imagef(entryPoints, smpUNormNoClampNearest, globalId);  
    
    float4 result = (float4)(0.f); 
    if(any(entry.xyz != 0.f)) {     
        float4 exit = read_imagef(exitPoints, smpUNormNoClampNearest, globalId);   
        float3 direction = exit.xyz - entry.xyz;   
        float tEnd = length(direction);
        float tIncr = 1.f/(stepSize*length(direction*convert_float3(volumeDimension)));
        direction = normalize(direction);
        float3 p = entry.xyz;
        float t = 0.0f; 
        //float t = 0.5f*tIncr; 
        float volumeSample;
        float extinction = 0.f;  
        while(t < tEnd) {
            float3 pos = entry.xyz+t*direction;
            volumeSample = read_imagef(volume, smpNormClampEdgeLinear, as_float4(pos)).x; 
            // xyz == emission, w = absorption
            float4 emissionAbsorption = read_imagef(transferFunction, smpNormClampEdgeLinear, (float2)(volumeSample, 0.5f));
            //emissionAbsorption.xyz *= 10.f;
            //float4 emissionAbsorption2 = read_imagef(transferFunction, smpNormClampLinear, (float2)(volumeSample, 0.5f));
            //float4 emissionAbsorption = (float4)((float3)(volumeSample*10.f), volumeSample)+0.00000001f*emissionAbsorption2;
            // Taylor expansion approximation
            emissionAbsorption.w = 1.f - pow(1.f - emissionAbsorption.w, tIncr * REF_SAMPLING_INTERVAL);
			result.xyz = result.xyz + (1.f - result.w) * emissionAbsorption.w * emissionAbsorption.xyz;
            result.w = result.w + (1.f - result.w) * emissionAbsorption.w;	
            
            // This code should be more correct but for some reason it makes the result vary with different sampling rates. 
            // Need to verify
            //emissionAbsorption.w = 1.f-pow(1.f - emissionAbsorption.w, tIncr * REF_SAMPLING_INTERVAL);
            ////emissionAbsorption.w = emissionAbsorption.w*tIncr*REF_SAMPLING_INTERVAL;            
            //
            //result.xyz += emissionAbsorption.w*emissionAbsorption.xyz*result.w;
            //extinction += emissionAbsorption.w;
            //result.w = exp(-extinction);

            if (result.w > ERT_THRESHOLD) t = tEnd;   
            else t += tIncr;   
 
        }
    }
         
    write_imagef(output, globalId,  result);     
  
}
  
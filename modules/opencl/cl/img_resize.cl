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

__constant sampler_t linearSamp = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR; 

__kernel void resizeImage(read_only image2d_t src, write_only image2d_t dst) 
{ 
    if (get_global_id(0) >= get_image_width(dst) || get_global_id(1) >= get_image_height(dst)) { return; } 

    float2 srcSize = (float2)(convert_float(get_image_width(src)), convert_float(get_image_height(src))); 
    float2 dstSize = (float2)(convert_float(get_image_width(dst)), convert_float(get_image_height(dst))); 
    float2 borderOffset = 0.5f/srcSize; 
    // Coordinate in [0 1] range "
    float2 dstCoord = (float2)(convert_float(get_global_id(0)), convert_float(get_global_id(1)))/(dstSize-1.f); 
    // Coordinate in [borderOffset (1-2*borderOffset)] range 
    float2 coord = borderOffset + dstCoord*(1.f-2.f*borderOffset);
   
    write_imagef(dst, (int2)(get_global_id(0), get_global_id(1)), read_imagef(src, linearSamp, coord));
};
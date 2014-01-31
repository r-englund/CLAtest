/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
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

// Trilinear interpolation 
// Example of how to get data values       
// float s[8];int i = 0;
// for(int z = zMin; z <= zMin+1; ++z) {
//     for(int y = yMin; y <= yMin+1; ++y) {
//          for(int x = xMin; x <= xMin+1; ++x) {
//              s[i++] = read_imagef(volume, smpUNormNoClampNearest, (int4)(x,y,z,0)).x;
//          }
//     }
// }
float trilinearInterploation(const float3 s, const float d[8]) {
	// Interpolate along z
	float i1 = mix(d[0], d[4], s.z);
	float i2 = mix(d[1], d[5], s.z);
	float i3 = mix(d[2], d[6], s.z);
	float i4 = mix(d[3], d[7], s.z);
	// interpolate along y 
	float w1 = mix(i1, i3, s.y);
	float w2 = mix(i2, i4, s.y);

	return mix(w1, w2, s.x);
}
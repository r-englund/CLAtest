#include "modules/mod_sampler2d.frag"

uniform TEXTURE_TYPE entryColorTex_;
uniform TEXTURE_TYPE entryDepthTex_;
uniform TEXTURE_PARAMETERS entryParameters_;
uniform TEXTURE_TYPE exitColorTex_;
uniform TEXTURE_TYPE exitDepthTex_;
uniform TEXTURE_PARAMETERS exitParameters_;

uniform mat4 NDCToTextureMat_; // Normalized device coordinates to volume texture coordinates
uniform float nearDist_;

void main() {
	float entryDepth = texture2D(entryDepthTex_, vec2(gl_TexCoord[0])).z;
	float exitDepth = texture2D(exitDepthTex_, vec2(gl_TexCoord[0])).z;

	vec4 entryColor;
	if (entryDepth > exitDepth) {
		// entry points are clipped by near plane
		// Convert texture coordinates to normalized device coordinates. (ndc) The z value will always be -1 on the clipping plane 
        vec4 cameraCoordinates = vec4(2.0f*gl_TexCoord[0].xy-1.0f, -1.0f, 1.0f);
		// convert the ndc back to the volume texture coordinates
	    entryColor = NDCToTextureMat_ * cameraCoordinates * nearDist_;
		entryDepth = 0.0f;
	} else {
		entryColor = texture2D(entryColorTex_, vec2(gl_TexCoord[0]));
	}
    
	FragData0 = entryColor;
    gl_FragDepth = entryDepth;
}
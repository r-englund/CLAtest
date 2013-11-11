#include "modules/mod_sampler2d.frag"

uniform TEXTURE_TYPE entryColorTex_;
uniform TEXTURE_TYPE entryDepthTex_;
uniform TEXTURE_PARAMETERS entryParameters_;
uniform TEXTURE_TYPE exitColorTex_;
uniform TEXTURE_TYPE exitDepthTex_;
uniform TEXTURE_PARAMETERS exitParameters_;

uniform mat4 inverseViewMat_;
uniform mat4 worldToTexMat_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * screenDimRCP_;

	float entryDepth = texture2D(entryDepthTex_, texCoords).z;
	float exitDepth = texture2D(exitDepthTex_, texCoords).z;

	vec4 entryColor;
	if (entryDepth > exitDepth) {
		// entry points are clipped by near plane
		vec4 cameraCoordinates = gl_TexCoord[0];
		vec4 worldCoordinates = cameraCoordinates*inverseViewMat_;
		vec4 texCoordinates = worldCoordinates*worldToTexMat_;
	    entryColor = cameraCoordinates;
		entryDepth = 0.0;
	} else {
		entryColor = texture2D(entryColorTex_, texCoords);
	}
    
	FragData0 = entryColor;
    gl_FragDepth = entryDepth;
}
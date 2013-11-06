
#ifndef WARD_BRDF_CL
#define WARD_BRDF_CL

#include "shading/shadingmath.cl"
#include "shading/microfacet.cl"

// not fully functional.
// TODO: Look at http://www.graphics.cornell.edu/~bjw/wardnotes.pdf

float WardIsotropicDistribution(const float3 wo, const float3 wi, const float3 wh, float r) {
    float tanThetaH;
    tanThetaH = sinTheta(wh) / absCosTheta(wh);
    float dfac = tanThetaH/r;
    return 0.5f * M_1_PI * native_exp(-(dfac * dfac)) / ( r * r * absCosTheta(wi)*absCosTheta(wo));

    //float NdotWo = absCosTheta(wo);
    //float NdotWi = absCosTheta(wi);
    //float theta = acos(NdotH);
    //float tanThetaH = tan(theta);
    ////r+= 1e-3;
    //float dfac = tanThetaH/r;

    //return exp(-(dfac * dfac)) / (M_PI * r * r * powr(NdotH, 3.f));
}

float WardBRDF(const float3 wo, const float3 wi, const float f0, const float alphaX, const float alphaY) {
	float3 wh = wo+wi;
	if (wh.z <= 0.f) return 0.f;
	float tmp = wi.z*wo.z;
	if (tmp <= 0.f) return 0.f;
    Microfacet distribution;
    wh = normalize(wh);
    float NdotWh = absCosTheta(wh);
    distribution.D = WardIsotropicDistribution(wo, wi, wh, alphaX);
    //distribution.D = BlinnPhongDistribution(NdotWh, m);
    distribution.G = CookTorranceGeometry(wo, wi, wh);
    distribution.F = fresnelSchlick(NdotWh, f0); 
    return microfacetBRDF(wo, wi, wh, &distribution);
	
	//return native_exp(-(pown(wh.x/alphaX, 2)+pown(wh.y/alphaY, 2))/(wh.z*wh.z)) / (4.f*M_PI*alphaX*alphaY*native_sqrt(wi.z*wo.z));
    //return exp(-(pown(wh.x/alphaX, 2)+pown(wh.y/alphaY, 2))/(wh.z*wh.z)) / (4.f*M_PI*alphaX*alphaY*native_sqrt(wi.z*wo.z));
}

float3 WardSample(const float3 wo, const float3 N, const float alphaX, const float alphaY, const float2 rnd) {
    float2 invAlpha2 = (float2)(1.f/(alphaX*alphaX), 1.f/(alphaY*alphaY));
	float phi = atan(alphaY*tan(2.f*M_PI*rnd.y)/alphaX);
	float cosphi = native_cos(phi);
	float sinphi = native_sqrt(1.f-cosphi*cosphi);
	float theta = atan( native_sqrt(-log(rnd.x)/(cosphi*cosphi*invAlpha2.x + sinphi*sinphi*invAlpha2.y)));
	float3 wh;
	wh.z = native_cos(theta);
	float cosTheta2 = wh.z*wh.z;
	float sintheta = native_sqrt(1.f-cosTheta2);
	float tanTheta2 = (1.f-cosTheta2)/cosTheta2;
	wh.x = cosphi*sintheta;
	wh.y = sinphi*sintheta;
	if (!sameHemisphereN(wo, wh, N)) wh = -wh;
	return -wo + 2.f*dot(wo, wh)*wh;
}


float WardPdf(const float3 wo, const float3 wi, const float3 N, const float alphaX, const float alphaY) {
    if ( !sameHemisphere(wo, wi) ) return 0.f; 
	    
    float2 invAlpha2 = (float2)(1.f/(alphaX*alphaX), 1.f/(alphaY*alphaY));
    float3 wh = normalize(wi+wo);
    float cosTheta2 = wh.z*wh.z;
    float tanTheta2 = (1.f-cosTheta2)/cosTheta2;
    float cosphi = cosPhi(wh);
    float sinphi = sinPhi(wh);

    return exp(-tanTheta2*(cosphi*cosphi*invAlpha2.x + sinphi*sinphi*invAlpha2.y))/((4.f*M_PI*alphaX*alphaY)*dot(wh,wo)*cosTheta2*wh.z);
}


#endif

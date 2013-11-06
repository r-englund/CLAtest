#ifndef MICROFACET_CL
#define MICROFACET_CL


// Schlick's approximation: http://en.wikipedia.org/wiki/Schlick%27s_approximation
// costheta is half the angle between the incoming and outgoing light directions (dot(L,H)), and R0 is the reflectance at normal incidence (i.e., the value of the Fresnel term when theta = 0)
// r0 = ( (1-n)/(1+n) ) ^ 2
float fresnelSchlick(const float costheta, const float r0) {
    // Using native_powr is faster than pown
    //return r0 + (1.f - r0)*pown( (1.f - costheta), 5 ); 
    return r0 + (1.f - r0)*pow(1.f - costheta, 5.f ); 
}

float fresnelDielectricR(const float etai, const float etat, const float cosi, const float cost) {
    float Rparl = pown((etat*cosi-etai*cost) / (etat*cosi+etai*cost), 2 );
    float Rperp = pown((etai*cosi-etat*cost) / (etai*cosi+etat*cost), 2 );
    return ( Rparl+Rperp ) / 2.f;
}
float fresnelDielectric(const float eta_i, const float eta_t, float cosi) {
    cosi = clamp(cosi, -1.f, 1.f);
    float ei = eta_i; float et = eta_t;
    // Swap if not entering 
    if ( cosi <= 0.f ) {
        float tmp = et;
        et = ei;
        ei = tmp;
    }
    // Compute sint using Snell's law
    float sint = ei/et * native_sqrt(max(0.f, 1.f-cosi*cosi));
    if( sint >= 1.f) 
        return 1.f;
    else {
        float cost = native_sqrt(max(0.f, 1.0f - sint * sint));
        return fresnelDielectricR(ei, et, fabs(cosi), cost);
    }

}


// Microfacet model (good reference: http://simonstechblog.blogspot.com/2011/12/microfacet-brdf.html)
// f_microfacet(w_i, w_o) = F(w_i,H)*G(w_i, w_o, H)*D(H) / ( 4*dot(N, w_i)*dot(N, w_o) )
// where F is Fresnel reflectance term
//       G is geometry term describing shadowing between microfacets
//       D is normal distribution term describing how microfaces normals are distributed around half angle H
//       w_i is incident light direction
//       w_o is the view direction
//       H is the half vector w_o+w_i


typedef struct Microfacet {
    float F;
    float G;
    float D;
} Microfacet;

float microfacetBRDF(const float3 wo, const float3 wi, const float3 wh, const Microfacet* microfacet) {
    //float cosThetaWo = absCosTheta(wo);
    //float cosThetaWi = absCosTheta(wi);
    float cosThetaWoCosThetaWi = absCosTheta(wo)*absCosTheta(wi);
    if(cosThetaWoCosThetaWi < 1e-6f) {
        return 0.f;
    }
    if ( wh.x == 0.f && wh.y == 0.f && wh.z == 0.f ) 
        return (float)(0.f);
    if(!isfinite(microfacet->D)) {
        return 0.f;
    }
    
    return microfacet->F * microfacet->G * microfacet->D  / (4.0f * cosThetaWoCosThetaWi);

}



// Blinn-Phong microfacet distribution
inline float BlinnPhongDistribution(const float NdotH, const float exponent) {
    return (exponent+2.f) * 0.5f * M_1_PI * native_powr(NdotH, exponent);
}
//inline float BlinnPhongDistribution(const float NdotH, const float exponent) {
//    return (exponent+4.f) * native_powr(NdotH, exponent)/(8.f*M_PI);
//}
// Beckmann microfacet distribution
inline float BeckmannDistribution(const float NdotH, const float m) {
    // D = exp( -tan^2(alpha)/m^2 ) / ( PI*m^2*cos^4(alpha) ) 
    // alpha = arccos(N*H)
    // cos(alpha) = cos(arccos(N*H)) = N*H 
    // Simplified through:
    // -tan^2(alpha)/m^2 = (cos^2(alpha)-1)/(m*m*cos(alpha))

    float NdotH2 = NdotH*NdotH;
    float m2NdotH2 = m*m*NdotH2;
    return native_exp( (NdotH2-1.f)/(m2NdotH2) ) / ( M_PI*m2NdotH2*NdotH2 );

}
// Adapted from luxrender
// Based on http://sirkan.iit.bme.hu/~szirmay/scook.pdf
float3 BeckmannSample(const float3 wo, float m, const float2 rnd) {
    // Compute sampled half-angle vector $\wh$ for Beckmann distribution
    // Adapted from B. Walter et al, Microfacet Models for Refraction, Eurographics Symposium on Rendering, 2007, page 7

    float theta = atan (native_sqrt (-(m * m) * log(1.f - rnd.x)));
    float costheta = cos (theta);
    float sintheta = native_sqrt(max(0.f, 1.f - costheta*costheta));
    float phi = rnd.y * 2.f * M_PI;

    float3 H = sphericalDirection(sintheta, costheta, phi);

    if (!sameHemisphere(wo, H))
        H.z *= -1.f;

    // Compute incident direction by reflecting about $\wh$
    return -wo + 2.f * dot(wo, H) * H;
}
// Adapted from luxrender
float BeckmannPdf(const float3 wo, const float3 wi, const float m) {
    // Compute PDF for \wi from Beckmann distribution - note that the inverse of the integral over
    // the Beckmann distribution is not available in closed form, so this is not really correct
    // (see Kelemen and Szirmay-Kalos / Microfacet Based BRDF Model, Eurographics 2001)
    float3 H = normalize(wo+wi);
    float conversion_factor = 1.f / (4.f * dot(wo, H));
    return conversion_factor * BeckmannDistribution(absCosTheta(H), m);
}



inline float CookTorranceGeometry(const float3 wo, const float3 wi, const float3 wh) {
    float NdotWh = absCosTheta(wh);
    float NdotWo = absCosTheta(wo);
    float NdotWi = absCosTheta(wi);
    float WOdotWh = fabs(dot(wo, wh));
    //if(WOdotWh < 0.00001f) return 1.f;
    // Important to use fmin and not min here since inf/NaN may be produced
    return fmin(1.f, fmin((2.f * NdotWh * NdotWo / WOdotWh),
                        (2.f * NdotWh * NdotWi / WOdotWh)));
}






#endif

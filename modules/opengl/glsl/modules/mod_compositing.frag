// set reference sampling interval for opacity correction
#define REF_SAMPLING_INTERVAL 150.0

vec4 compositeDVR(in vec4 curResult, in vec4 color, in float t, inout float tDepth, in float tIncr) {
    vec4 result = curResult;
    if (tDepth == -1.0 && color.a > 0.0)
        tDepth = t;
    color.a = 1.0 - pow(1.0 - color.a, tIncr * REF_SAMPLING_INTERVAL);
    result.rgb = result.rgb + (1.0 - result.a) * color.a * color.rgb;
    result.a = result.a + (1.0 - result.a) * color.a;
    return result;
}

vec4 compositeMIP(in vec4 curResult, in vec4 color, in float t, inout float tDepth) {
    vec4 result = curResult;
    if (color.a > curResult.a) {
        tDepth = t;
        result = color;
    }
    return result;
}

vec4 compositeFHP(in vec4 curResult, in vec4 color, in vec3 samplePos, in float t, inout float tDepth) {
    vec4 result = curResult;
    if (result == vec4(0.0) && color.a > 0.0) {
        tDepth = t;
        result = vec4(samplePos, 1.0);
    }
    return result;
}

vec4 compositeFHN(in vec4 curResult, in vec4 color, in vec3 gradient, in float t, inout float tDepth) {
    vec4 result = curResult;
    if (result == vec4(0.0) && color.a > 0.0) {
        tDepth = t;
        result = vec4(normalize(gradient)*0.5+0.5, 1.0);
    }
    return result;
}

vec4 compositeISO(in vec4 curResult, in vec4 color, in float t, inout float tDepth, in float tIncr, in float isoValue) {
    vec4 result = curResult;
    if (color.a >= isoValue-0.01 && color.a <= isoValue+0.01) {
        if (tDepth == -1.0)
            tDepth = t;
        color.a = 1.0 - pow(1.0 - color.a, tIncr * REF_SAMPLING_INTERVAL);
        result.rgb = result.rgb + (1.0 - result.a) * color.a * color.rgb;
        result.a = result.a + (1.0 - result.a) * color.a;
    }
    return result;
}

vec4 compositeISON(in vec4 curResult, in vec4 color, in vec3 gradient, in float t, inout float tDepth, in float isoValue) {
    vec4 result = curResult;
    if (color.a >= isoValue-0.01 && color.a <= isoValue+0.01) {
        result = compositeFHN(curResult, color, gradient, t, tDepth);
    }
    return result;
}
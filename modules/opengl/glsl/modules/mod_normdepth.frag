uniform float depthMin_;
uniform float depthMax_;

float normalizeDepth(float depth) {
    float result = depth - depthMin_;
    result /= (depthMax_ - depthMin_);
    return result;
}

float normDepth(float depth) {
    // deprecated
    return normalizeDepth(depth);
}
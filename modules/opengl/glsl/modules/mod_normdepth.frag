uniform float depthMin_;
uniform float depthMax_;

float normDepth(float depth) {
    normalizeDepth(depth);
}

float normalizeDepth(float depth) {
    float result = depth - depthMin_;
    result /= (depthMax_ - depthMin_);
    return result;
}
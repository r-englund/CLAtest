uniform float minDepth_;
uniform float maxDepth_;

float normalizeDepth(float depth) {
    return depth;
    float result = depth - minDepth_;
    result /= (maxDepth_ - minDepth_);
    return result;
}

float normDepth(float depth) {
    // deprecated
    return normalizeDepth(depth);
}
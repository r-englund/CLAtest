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

float calculateDepthValue(float t, float minDepth, float maxDepth) {
    // FIXME: implement correct depth value caluclation
    return (minDepth + (t*(maxDepth-minDepth)));
}
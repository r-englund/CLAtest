/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/geometry/marchingtetrahedron.h>

namespace inviwo {

MarchingTetrahedron::MarchingTetrahedron(const VolumeRepresentation *in, const double &iso,
                                         const vec4 &color)
    : VolumeOperation(in), iso_(iso), color_(color) {}

static glm::vec3 interpolateASDF(const glm::vec3 &p0, const double &v0, const glm::vec3 &p1,
                                 const double &v1) {
    float t = 0;
    if (v0 != v1) t = v0 / (v0 - v1);
    return t * p1 + (1 - t) * p0;
}

void MarchingTetrahedron::evaluateTetra(IndexBufferRAM *indexBuffer, const glm::vec3 &p0,
                                        const double &v0, const glm::vec3 &p1, const double &v1,
                                        const glm::vec3 &p2, const double &v2, const glm::vec3 &p3,
                                        const double &v3) {
    int index = 0;
    if (v0 > 0) index += 1;
    if (v1 > 0) index += 2;
    if (v2 > 0) index += 4;
    if (v3 > 0) index += 8;
    glm::vec3 a, b, c, d;
    if (index == 0 || index == 15)
        return;
    if (index == 1 || index == 14) {
        a = interpolateASDF(p0, v0, p2, v2);
        b = interpolateASDF(p0, v0, p1, v1);
        c = interpolateASDF(p0, v0, p3, v3);
        if (index == 1) {
            addTriangle(indexBuffer, a, b, c);
        } else {
            addTriangle(indexBuffer, a, c, b);
        }
    } else if (index == 2 || index == 13) {
        a = interpolateASDF(p1, v1, p0, v0);
        b = interpolateASDF(p1, v1, p2, v2);
        c = interpolateASDF(p1, v1, p3, v3);
        if (index == 2) {
            addTriangle(indexBuffer, a, b, c);
        } else {
            addTriangle(indexBuffer, a, c, b);
        }

    } else if (index == 4 || index == 11) {
        a = interpolateASDF(p2, v2, p0, v0);
        b = interpolateASDF(p2, v2, p1, v1);
        c = interpolateASDF(p2, v2, p3, v3);
        if (index == 4) {
            addTriangle(indexBuffer, a, c, b);
        } else {
            addTriangle(indexBuffer, a, b, c);
        }
    } else if (index == 7 || index == 8) {
        a = interpolateASDF(p3, v3, p0, v0);
        b = interpolateASDF(p3, v3, p2, v2);
        c = interpolateASDF(p3, v3, p1, v1);
        if (index == 7) {
            addTriangle(indexBuffer, a, b, c);
        } else {
            addTriangle(indexBuffer, a, c, b);
        }
    } else if (index == 3 || index == 12) {
        a = interpolateASDF(p0, v0, p2, v2);
        b = interpolateASDF(p1, v1, p3, v3);
        c = interpolateASDF(p0, v0, p3, v3);
        d = interpolateASDF(p1, v1, p2, v2);

        if (index == 3) {
            addTriangle(indexBuffer, a, b, c);
            addTriangle(indexBuffer, a, d, b);
        } else {
            addTriangle(indexBuffer, a, c, b);
            addTriangle(indexBuffer, a, b, d);
        }

    } else if (index == 5 || index == 10) {
        a = interpolateASDF(p2, v2, p3, v3);
        b = interpolateASDF(p0, v0, p1, v1);
        c = interpolateASDF(p0, v0, p3, v3);
        d = interpolateASDF(p1, v1, p2, v2);

        if (index == 5) {
            addTriangle(indexBuffer, a, b, c);
            addTriangle(indexBuffer, a, d, b);
        } else {
            addTriangle(indexBuffer, a, c, b);
            addTriangle(indexBuffer, a, b, d);
        }

    } else if (index == 6 || index == 9) {
        a = interpolateASDF(p1, v1, p3, v3);
        b = interpolateASDF(p0, v0, p2, v2);
        c = interpolateASDF(p0, v0, p1, v1);
        d = interpolateASDF(p2, v2, p3, v3);

        if (index == 6) {
            addTriangle(indexBuffer, a, c, b);
            addTriangle(indexBuffer, a, b, d);
        } else {
            addTriangle(indexBuffer, a, b, c);
            addTriangle(indexBuffer, a, d, b);
        }
    }
}

unsigned MarchingTetrahedron::addVertex(const vec3 pos) {
    K3DTree<unsigned>::Node *nearest = vertexTree_.findNearest(vec3(pos));
    vec3 p;
    if (nearest) {
        p.x = nearest->getPosition()[0];
        p.y = nearest->getPosition()[1];
        p.z = nearest->getPosition()[2];
    }
    if (!nearest || (glm::distance(p, pos) > glm::epsilon<double>() * 5)) {
        nearest = vertexTree_.insert(vec3(pos), static_cast<unsigned>(positions_.size()));
        positions_.push_back(pos);
        normals_.push_back(vec3(0, 0, 0));
    }
    return nearest->get();
}

void MarchingTetrahedron::addTriangle(IndexBufferRAM *indexBuffer, const glm::vec3 &a,
                                      const glm::vec3 &b, const glm::vec3 &c) {
    unsigned i0 = addVertex(a);
    unsigned i1 = addVertex(b);
    unsigned i2 = addVertex(c);

    if (i0 == i1 || i0 == i2 || i1 == i2) {
        // triangle is so small so that the vertices are merged.
        return;
    }

    indexBuffer->add(i0);
    indexBuffer->add(i1);
    indexBuffer->add(i2);

    vec3 e0 = b - a;
    vec3 e1 = c - a;
    vec3 n = glm::normalize(glm::cross(e0, e1));

    normals_[i0] += n;
    normals_[i1] += n;
    normals_[i2] += n;
}

}  // namespace

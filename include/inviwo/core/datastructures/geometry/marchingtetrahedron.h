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

#ifndef IVW_MARCHINGTETRAHEDRON_H
#define IVW_MARCHINGTETRAHEDRON_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/datastructures/image/layerramprecision.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>
#include <modules/experimental/datastructures/kdtree.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>

namespace inviwo {
    
class IVW_CORE_API MarchingTetrahedron : public VolumeOperation {
public:
    MarchingTetrahedron(const VolumeRepresentation *in, const double &iso,const vec4 &color);
    virtual ~MarchingTetrahedron() {}


    template <typename T, size_t B>
    void evaluate();

    static inline Geometry *apply(const VolumeRepresentation *in,const double &iso,const vec4 &color) {
        MarchingTetrahedron marchingOP = MarchingTetrahedron(in, iso, color);
        in->performOperation(&marchingOP);
        return marchingOP.getOutput<Geometry>();
    }

private:
    double iso_;
    K3DTree<unsigned> vertexTree_;
    std::vector<vec3> positions_;
    std::vector<vec3> normals_;
    vec4 color_;

    void evaluateTetra(IndexBufferRAM *indexBuffer, const glm::vec3 &p0, const double &v0,
                       const glm::vec3 &p1, const double &v1, const glm::vec3 &p2, const double &v2,
                       const glm::vec3 &p3, const double &v3);

    unsigned addVertex(const vec3 pos);
    void addTriangle(IndexBufferRAM *indexBuffer, const glm::vec3 &a, const glm::vec3 &b,
                     const glm::vec3 &c);

    template <typename T>
    double getValue(const T *src, uvec3 pos, uvec3 dim);

    template <typename T>
    static double toSingle(const glm::detail::tvec2<T, glm::defaultp> &v) {
        return v.x;
    }
    template <typename T>
    static double toSingle(const glm::detail::tvec3<T, glm::defaultp> &v) {
        return v.x;
    }
    template <typename T>
    static double toSingle(const glm::detail::tvec4<T, glm::defaultp> &v) {
        return v.x;
    }

    template <typename T>
    static double toSingle(const T &v) {
        return v;
    }
};

template <typename T>
double MarchingTetrahedron::getValue(const T *src, uvec3 pos, uvec3 dim) {
    double v = toSingle(src[VolumeRAM::posToIndex(pos, dim)]);
    return - (v - iso_);
}

template <typename T, size_t B>
void MarchingTetrahedron::evaluate() {
    vertexTree_.clear();
    positions_.clear();
    normals_.clear();

    const VolumeRAMPrecision<T> *volume =
        dynamic_cast<const VolumeRAMPrecision<T> *>(getInputVolume());

    if (!volume) {
        setOutput(NULL);
        return;
    }

    BasicMesh *mesh = new BasicMesh();
    IndexBufferRAM *indexBuffer =
        mesh->addIndexBuffer(GeometryEnums::TRIANGLES, GeometryEnums::NONE);
    mesh->initialize();

    const Volume *baseVolume = dynamic_cast<const Volume *>(volume->getOwner());
    if (baseVolume) {
        mesh->setBasisAndOffset(baseVolume->getBasisAndOffset());
        mesh->setWorldTransform(baseVolume->getWorldTransform());
    }
    setOutput(mesh);

    const T *src = reinterpret_cast<const T *>(volume->getData());

    uvec3 dim = volume->getDimension();
    double x, y, z, dx, dy, dz;
    dx = 1.0f / (dim.x - 1);
    dy = 1.0f / (dim.y - 1);
    dz = 1.0f / (dim.z - 1);
    double v[8];
    glm::vec3 p[8];

    bool single = volume->getDataFormat()->getComponents() == 1;

    const static unsigned int tetras[6][4] = {
        {0, 1, 3, 5}, {1, 2, 3, 5}, {2, 3, 5, 6}, {0, 3, 4, 5}, {7, 4, 3, 5}, {7, 6, 5, 3}};

    for (unsigned k = 0; k < dim.z - 1; k++) {
        for (unsigned j = 0; j < dim.y - 1; j++) {
            for (unsigned i = 0; i < dim.x - 1; i++) {
                x = dx * i;
                y = dy * j;
                z = dz * k;

                p[0] = glm::vec3(x, y, z);
                p[1] = glm::vec3(x + dx, y, z);
                p[2] = glm::vec3(x + dx, y + dy, z);
                p[3] = glm::vec3(x, y + dy, z);
                p[4] = glm::vec3(x, y, z + dz);
                p[5] = glm::vec3(x + dx, y, z + dz);
                p[6] = glm::vec3(x + dx, y + dy, z + dz);
                p[7] = glm::vec3(x, y + dy, z + dz);

                v[0] = getValue(src, uvec3(i, j, k), dim);
                v[1] = getValue(src, uvec3(i + 1, j, k), dim);
                v[2] = getValue(src, uvec3(i + 1, j + 1, k), dim);
                v[3] = getValue(src, uvec3(i, j + 1, k), dim);
                v[4] = getValue(src, uvec3(i, j, k + 1), dim);
                v[5] = getValue(src, uvec3(i + 1, j, k + 1), dim);
                v[6] = getValue(src, uvec3(i + 1, j + 1, k + 1), dim);
                v[7] = getValue(src, uvec3(i, j + 1, k + 1), dim);

                bool ok = true;
                for (int ii = 0; ii < 8 && ok; ii++) {
                    ok = false;
                    if (v[ii] != v[ii]) break;
                    if (v[ii] == std::numeric_limits<float>::infinity()) break;
                    if (v[ii] == std::numeric_limits<float>::max()) break;
                    if (v[ii] == std::numeric_limits<float>::min()) break;
                    if (v[ii] == std::numeric_limits<double>::infinity()) break;
                    if (v[ii] == std::numeric_limits<double>::max()) break;
                    if (v[ii] == std::numeric_limits<double>::min()) break;
                    ok = true;
                }
                if (!ok) continue;

                for (int a = 0; a < 6; a++) {
                    evaluateTetra(indexBuffer, p[tetras[a][0]], v[tetras[a][0]], p[tetras[a][1]],
                                  v[tetras[a][1]], p[tetras[a][2]], v[tetras[a][2]],
                                  p[tetras[a][3]], v[tetras[a][3]]);
                }
            }
        }
    }

    ivwAssert(positions_.size() == normals_.size(), "positions_ and normals_ must be equal");
    std::vector<vec3>::iterator P = positions_.begin();
    std::vector<vec3>::iterator N = normals_.begin();
    for (; P != positions_.end(); ++P, ++N) {
        mesh->addVertex(*P, glm::normalize(*N), *P, color_);
    }
}

}  // namespace

#endif  // IVW_MARCHINGTETRAHEDRON_H

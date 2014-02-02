/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_EDGE_H
#define IVW_EDGE_H

namespace inviwo {

template<typename T>
class Edge {

public:
    T v1, v2;

    Edge(T in1) : v1(in1), v2(in1) {};

    Edge(T in1, T in2) : v1(in1), v2(in2) {};

    bool operator==(const Edge<T>& e) const {
        return ((this->v1==e.v1) && (this->v2==e.v2)) || ((this->v1==e.v2) && (this->v2==e.v1));
    }

protected:
    //Protected as it can't instantiate v1 and v2;
    Edge() {}

};

template<typename T, size_t B>
class EdgeDataFormat : public Edge<T> {

public:
    EdgeDataFormat() : Edge<T>() {
        DataFormat<T,B>::get()->floatToValue(0.f, &this->v1);
        this->v2 = this->v1;
    }

    EdgeDataFormat(T in1) : Edge<T>(in1) {};
    EdgeDataFormat(T in1, T in2) : Edge<T>(in1, in2) {};

};

#define DataFormatEdge(D) EdgeDataFormat<D::type, D::bits>

typedef DataFormatEdge(DataUINT32) EdgeIndex;
typedef DataFormatEdge(DataVec2FLOAT32) Edge2D;
typedef DataFormatEdge(DataVec3FLOAT32) Edge3D;

} // namespace

#endif // IVW_EDGE_H
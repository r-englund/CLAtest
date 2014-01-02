/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_LAYERDISK_H
#define IVW_LAYERDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/diskrepresentation.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>

namespace inviwo {

class IVW_CORE_API LayerDisk : public LayerRepresentation, public DiskRepresentation {

public:
    LayerDisk();
	LayerDisk(std::string url);
    virtual ~LayerDisk();
    virtual void initialize();
    virtual void deinitialize();
    virtual void resize(uvec2 dimensions);
    virtual LayerDisk* clone() const;
    virtual std::string getClassName() const { return "LayerDisk"; };
    virtual bool copyAndResizeLayer(DataRepresentation*){ return false;};
    /** 
     * \brief loads data from url.
     *
     * @return void* return the raw data
     */
    void* loadFileData() const;
    /** 
     * \brief loads and rescales data from url.
     * 
     * @param uvec2 dst_dimesion destination dimension
     * @return void* returns the raw data that has been rescaled to dst_dimension
     */
    void* loadFileDataAndRescale(uvec2 dst_dimesion) const;
};

} // namespace

#endif // IVW_LAYERDISK_H

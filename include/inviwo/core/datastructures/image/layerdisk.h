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
    LayerDisk(LayerType type = COLOR_LAYER);
	LayerDisk(std::string url, LayerType type = COLOR_LAYER);
    LayerDisk(const LayerDisk& rhs);
    LayerDisk& operator=(const LayerDisk& that);
    virtual LayerDisk* clone() const;
    virtual ~LayerDisk();
    virtual void initialize();
    virtual void deinitialize();
    virtual std::string getClassName() const;
    virtual bool copyAndResizeLayer(DataRepresentation*) const;
    virtual void resize(uvec2);
    /** 
     * \brief loads data from url.
     *
     * @param void* the destination of the raw data
     * @param uvec2 dimension of the loaded image
     * @param DataFormatId the loaded format
     *
     */
    void* loadFileData(void*, uvec2&, DataFormatId&) const;
    /** 
     * \brief loads and rescales data from url.
     * 
     * @param void* the destination of the raw data
     * @param uvec2 dimension to scale image to
     * @param DataFormatId the loaded format
     *
     */
    void* loadFileDataAndRescale(void*, uvec2, DataFormatId&) const;
};

} // namespace

#endif // IVW_LAYERDISK_H

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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_IMAGECL_H
#define IVW_IMAGECL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/image/layercl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API ImageCL : public ImageRepresentation {

public:
    ImageCL();
    ImageCL(const ImageCL& other);
    virtual ~ImageCL();
    virtual std::string getClassName() const { return "ImageCL"; }
    virtual ImageCL* clone() const;
    virtual void initialize();
    virtual void deinitialize();

    LayerCL* getLayerCL();
    const LayerCL* getLayerCL() const;
    
    virtual bool copyAndResizeRepresentation(DataRepresentation*) const;

protected:
    virtual void update(bool);

    LayerCL* layerCL_;
};

} // namespace

namespace cl {

// Kernel argument specializations for ImageCL type
// Will set the color layer as argument.
// (enables calling cl::Queue::setArg with ImageCL)
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCL& value);

} // namespace cl

#endif // IVW_IMAGECL_H

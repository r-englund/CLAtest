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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_BACKGROUND_H
#define IVW_BACKGROUND_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API Background : public ProcessorGL {
public:
    Background();
    ~Background();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

	virtual void initializeResources();

protected:
    virtual void process();
    virtual bool isReady()const;
private:
    ImageInport inport_;
    ImageOutport outport_;

	OptionPropertyInt backgroundStyle_;
	FloatVec4Property color1_;
	FloatVec4Property color2_;

    Shader* shader_;

    bool hadData_;
};

} // namespace

#endif // IVW_BACKGROUND_H

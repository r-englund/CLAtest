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

#ifndef IVW_SHADERMANAGER_H
#define IVW_SHADERMANAGER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/openglcapabilities.h>
#include <modules/opengl/glwrap/shader.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ShaderManager : public Singleton<ShaderManager>, public FileObserver {

public:
    ShaderManager();

    void registerShader(Shader* shader);
    void unregisterShader(Shader* shader);

    virtual void fileChanged(std::string shaderFilename);

    std::string getGlobalGLSLHeader();
    std::string getGlobalGLSLFragmentDefines();

    std::vector<std::string> getShaderSearchPaths() { return shaderSearchPaths_; }
    void addShaderSearchPath(std::string shaderSearchPath) {
        shaderSearchPaths_.push_back(shaderSearchPath);
    }

private:
    std::vector<Shader*> shaders_;
    OpenGLCapabilities* openGLInfoRef_;
    std::vector<std::string> shaderSearchPaths_;
};

} // namespace

#endif // IVW_SHADERMANAGER_H

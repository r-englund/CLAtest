/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Contact: Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_URLPARSER_H
#define IVW_URLPARSER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API URLParser {
public:
    URLParser() {}

    /** 
     * \brief Adds the InviwoApplication base path before the url
     *
     * @see InviwoApplication::getBasePath
     * @param const std::string url Relative path
     * @return std::string InviwoApplication base path + url
     */
    static std::string addBasePath(const std::string url);

    static std::string getFileDirectory(const std::string& url);
    static std::string getFileNameWithExtension(const std::string& url);
    static std::string getFileNameWithoutExtension(const std::string& url);
    static std::string getFileExtension(const std::string& url);
    static std::string replaceFileExtension(const std::string& url, const std::string& newFileExtension);
    static std::string getRelativePath(const std::string& basePath, const std::string& absolutePath);
    static bool isAbsolutePath(const std::string& path);
    static bool sameDrive(const std::string& absPath1, const std::string absPath2);
};

} // namespace

#endif // IVW_URLPARSER_H

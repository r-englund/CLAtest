/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_FILE_SYSTEM_H
#define IVW_FILE_SYSTEM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

namespace filesystem {

/**
    * Get the working directory of the application.
    *
    * @note getBasePath should be used in the framework in general.
    * @see getBasePath
    * @return Full path to working directory.
    */
IVW_CORE_API std::string getWorkingDirectory();

/**
 * Check if a directory exists.
 *
 * @param path Directory path
 * @return True if found, otherwise false.
 */
IVW_CORE_API bool directoryExists(const std::string& path);

/**
 * Searches all parent folders of path and looks for parentFolder.
 *
 * @param path Folders to search, for example C:/a/b/c
 * @param parentFolder Folder to find.
 * @return The directory where the folder was found if found, otherwise path.
 */
IVW_CORE_API std::string getParentFolderPath(const std::string& path, const std::string& parentFolder);

/**
 * Finds base path which contains subfolders such as "data" and "modules" where external files are stored
 *
 * @return The directory considered to be the basePath.
 */
IVW_CORE_API std::string findBasePath();

} // namespace

} // namespace

#endif // IVW_FILE_SYSTEM_H

/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
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

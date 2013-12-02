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

#ifndef IVW_TEXTFILEREADER_H
#define IVW_TEXTFILEREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <iostream>
#include <string>


namespace inviwo {
/** \class TextFileReader 
*
* Reader for text files.
*/
class IVW_CORE_API TextFileReader {

public:
    TextFileReader(const std::string& filePath);
    virtual ~TextFileReader() {}

    /**
     * Read a file and return the content as a string.
     * Throws an exception if file could not be opened 
     * and prints an error message to the error log.
     * @param filePath Path of file.
     * @return Content of file.
     */
    virtual std::string read(const std::string& filePath) throw (std::ifstream::failure);

    /**
     * Read a file and return the content as a string.
     * Throws an exception if file could not be opened 
     * and prints an error message to the error log.
     * @return Content of file.
     */
    virtual std::string read() throw (std::ifstream::failure);
    
    void setFilePath(const std::string& filePath) { filePath_ = filePath; }
    const std::string& getFilePath() const { return filePath_; }
private:
    std::string filePath_;
};

} // namespace

#endif // IVW_TEXTFILEREADER_H

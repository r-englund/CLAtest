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

#include <inviwo/core/io/textfilereader.h>

namespace inviwo {

TextFileReader::TextFileReader(const std::string& filePath): filePath_(filePath)
{}

std::string TextFileReader::read( const std::string& filePath ) throw (std::ifstream::failure)
{
    filePath_ = filePath;
    return read();
}

std::string TextFileReader::read() throw (std::ifstream::failure)
{
    std::ifstream file;
    // Make ifstream throw exception if file could not be opened
    file.exceptions ( std::ifstream::failbit );
    try {
        file.open(filePath_.c_str());
    } catch (std::ifstream::failure& ex) {
        LogError("Error opening file " << filePath_);
        throw ex;
    }
    

    std::string data(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    if(data.empty()) {
        // When editing files using Visual Studio it may happen that the file is empty.
        // Wait a bit and hope that the content is there later. 
#ifdef WIN32
        Sleep(400);
#endif
        std::ifstream srcFile(filePath_.c_str());
        data = std::string(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    }
    file.close();
    return data;
}



} // namespace

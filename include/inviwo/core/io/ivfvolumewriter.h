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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_IVFVOLUMEWRITER_H
#define IVW_IVFVOLUMEWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/volumewriter.h>
#include <inviwo/core/io/ivfwritersettings.h>
#include <inviwo/core/util/filedirectory.h>

namespace inviwo {

class IVW_CORE_API IvfVolumeWriter : public VolumeWriter {
public:        

    IvfVolumeWriter(const std::string filePath, IvfWriterSettings writerSettings);

    virtual ~IvfVolumeWriter() {}

    virtual void writeData();

    static void writeIvfFileSettings(const std::string filePath, IvfWriterSettings& ivfWriterSettings)  {        
        //Write the ivf file content
        //Note: Raw file writing need to be explicitly called.       

        std::string rawFileAbsolutePath = ivfWriterSettings.rawFileAbsolutePath_;

        //for serialization absolute path is not needed, so replace absolute path
        ivfWriterSettings.rawFileAbsolutePath_ = URLParser::getFileNameWithExtension(ivfWriterSettings.rawFileAbsolutePath_);        

        std::string fileExtension = URLParser::getFileExtension(filePath);

        ivwAssert(fileExtension=="ivf", "should be a *.ivf file");

        if (fileExtension=="ivf") {

            std::string format=ivfWriterSettings.dataFormat_;
            if (ivfWriterSettings.dataFormat_== DataUINT8::str()) {
                ivfWriterSettings.dataFormat_ = "UCHAR";
            }
            else if (ivfWriterSettings.dataFormat_== DataUINT16::str()) {
                ivfWriterSettings.dataFormat_ = "USHORT";
            }        
           

            //Read the ivf file content
            IvwSerializer s(filePath);
            ivfWriterSettings.serialize(s);
            s.writeFile();

            //revert back absolute path which may be used by others
            ivfWriterSettings.rawFileAbsolutePath_ = rawFileAbsolutePath;
             ivfWriterSettings.dataFormat_ = format;
        }

        
    }

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    IvfWriterSettings writerFileSettings_; //Write file settings
    void writeRawVolumeData();
};  
} // namespace

#endif // IVW_IVFVOLUMEWRITER_H

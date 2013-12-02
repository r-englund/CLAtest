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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_DISKREPRESENTATION_H
#define IVW_DISKREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <string>
#include <inviwo/core/io/datareader.h>

namespace inviwo {

    class IVW_CORE_API DiskRepresentation {

    public:
        DiskRepresentation() : sourceFile_(""), reader_(NULL) {};
        DiskRepresentation(std::string srcFile) : sourceFile_(srcFile), reader_(NULL) {};
        virtual ~DiskRepresentation(){
            if(reader_){
                delete reader_;
            }
        };
        const std::string& getSourceFile() const { return sourceFile_; };
        bool hasSourceFile() const { return !sourceFile_.empty(); };

        void setDataReader(DataReader* reader){
            if(reader_){
                delete reader_;
            }
            reader_ = reader;
        }
        DataReader* getDataReader() const{
            return reader_;
        };
	private:
		std::string sourceFile_;      
        // DiskRepresentation owns a DataReader to be able to convert it self into RAM.
        DataReader* reader_;
    };

} // namespace

#endif // IVW_DISKREPRESENTATION_H

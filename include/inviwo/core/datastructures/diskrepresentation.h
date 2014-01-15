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
    DiskRepresentation();
    DiskRepresentation(std::string);
    DiskRepresentation(const DiskRepresentation& rhs);
    DiskRepresentation& operator=(const DiskRepresentation& that);
    virtual DiskRepresentation* clone() const;
    virtual ~DiskRepresentation();

    const std::string& getSourceFile() const;
    bool hasSourceFile() const;

    void setDataReader(DataReader* reader);

    void* readData() const;
    void readDataInto(void* dest) const;

private:
	std::string sourceFile_;      
    // DiskRepresentation owns a DataReader to be able to convert it self into RAM.
    DataReader* reader_;
};

} // namespace

#endif // IVW_DISKREPRESENTATION_H

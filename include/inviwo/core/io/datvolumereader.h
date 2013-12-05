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

#ifndef IVW_DATVOLUMEREADER_H
#define IVW_DATVOLUMEREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

/** \brief Reader for *.dat files 
 *
 *  The following tags are supported:
 *   - ObjectFileName :: The name of the raw data file, should be in the same directory (Mandatory).
 *   - Resolution | Dimension ::The size of the data grid: nx,ny,nz (Mandatory).
 *   - Format :: The type of values in the raw file. (Mandatory)
 *   - Spacing | SliceThickness :: The size of the voxels in the data. (Optional)
 *   - BasisVector(1|2|3) :: Defines a coordinate system for the data. (Optional, overides spacing, default: 2*IdentityMatrix);
 *   - Offset :: Offsets the basisvecors in space. (Optional, defaults to center the data on origo)
 *   - WorldVector(1|2|3|4) :: Defines a world transformation matrix that is applied last to orient the data in world space. (Optional, default: IdentityMatrix) 
 *
 *  The tag names are case insensitive and should always be followed by a ":" 
 */
class IVW_CORE_API DatVolumeReader : public DataReaderType<Volume> {
public:        
    DatVolumeReader();
    DatVolumeReader(const DatVolumeReader& rhs);
    DatVolumeReader& operator=(const DatVolumeReader& that);
    virtual DatVolumeReader* clone() const;
    virtual ~DatVolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;

private:
    std::string rawFile_;
    glm::uvec3 dimension_;
    const DataFormatBase* format_;
};    

} // namespace

#endif // IVW_DATVOLUMEREADER_H

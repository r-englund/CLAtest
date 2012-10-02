/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2011 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "metadatafactory.h"

#include "aggregationmetadata.h"
#include "positionmetadata.h"
#include "primitivemetadata.h"
#include "selectionmetadata.h"
#include "windowstatemetadata.h"
#include "zoommetadata.h"

namespace voreen {

const std::string MetaDataFactory::getTypeString(const std::type_info& type) const {
    if (type == typeid(BoolMetaData))
        return "BoolMetaData";
    else if (type == typeid(StringMetaData))
        return "StringMetaData";
    else if (type == typeid(PrimitiveMetaDataBase<std::string>))
        return "StringMetaData";
    else if (type == typeid(IntMetaData))
        return "IntMetaData";
    else if (type == typeid(SizeTMetaData))
        return "SizeTMetaData";
    else if (type == typeid(FloatMetaData))
        return "FloatMetaData";
    else if (type == typeid(PrimitiveMetaDataBase<float>))
        return "FloatMetaData";
    else if (type == typeid(DoubleMetaData))
        return "DoubleMetaData";
    else if (type == typeid(Vec3MetaData))
        return "Vec3MetaData";
    else if (type == typeid(PrimitiveMetaDataBase<tgt::vec3>))
        return "Vec3MetaData";
    else if (type == typeid(Mat4MetaData))
        return "Mat4MetaData";
    else if (type == typeid(PrimitiveMetaDataBase<tgt::mat4>))
        return "Mat4MetaData";
    else if (type == typeid(AggregationMetaDataContainer))
        return "AggregationMetaDataContainer";
    else if (type == typeid(PositionMetaData))
        return "PositionMetaData";
    else if (type == typeid(WindowStateMetaData))
        return "WindowStateMetaData";
    else if (type == typeid(SelectionMetaData<inviwo::Processor*>))
        return "SelectionMetaData::Processor";
    else if (type == typeid(ZoomMetaData))
        return "ZoomMetaData";
    else 
        return "";
}

Serializable* MetaDataFactory::createType(const std::string& typeString) {
    if (typeString == "BoolMetaData")
        return new BoolMetaData();
    else if (typeString == "StringMetaData")
        return new StringMetaData();
    else if (typeString == "IntMetaData")
        return new IntMetaData();
    else if (typeString == "SizeTMetaData")
        return new SizeTMetaData();
    else if (typeString == "FloatMetaData")
        return new FloatMetaData();
    else if (typeString == "DoubleMetaData")
        return new DoubleMetaData();
    else if (typeString == "Vec3MetaData")
        return new Vec3MetaData();
    else if (typeString == "Mat4MetaData")
        return new Mat4MetaData();
    else if (typeString == "AggregationMetaDataContainer")
        return new AggregationMetaDataContainer();
    else if (typeString == "PositionMetaData")
        return new PositionMetaData();
    else if (typeString == "WindowStateMetaData")
        return new WindowStateMetaData();
    else if (typeString == "SelectionMetaData::Processor")
        return new SelectionMetaData<inviwo::Processor*>();
    else if (typeString == "ZoomMetaData")
        return new ZoomMetaData();
    else
        return 0;
}

} // namespace voreen

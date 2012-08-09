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

#ifndef VRN_AGGREGATIONMETADATA_H
#define VRN_AGGREGATIONMETADATA_H

#include "../serialization.h"
#include "inviwo/core/network/processornetwork.h"
#include "inviwo/core/network/portconnection.h"


namespace voreen {

class AggregationMetaData : public Serializable {
public:
    AggregationMetaData();
    virtual ~AggregationMetaData();

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

    void setProcessors(const std::vector<inviwo::Processor*>& processors);
    std::vector<inviwo::Processor*> getProcessors(bool recursive = false) const;

    void addAggregation(AggregationMetaData* aggregation);
    void setAggregations(const std::vector<AggregationMetaData*>& aggregations);
    const std::vector<AggregationMetaData*>& getAggregations() const;

    void addPortConnection(inviwo::Port* outport, inviwo::Port* inport);
    const std::vector<inviwo::PortConnection>& getPortConnections() const;

    void setPosition(const int& x, const int& y);
    std::pair<int,int> getPosition() const;

    void setName(const std::string& name);
    const std::string& getName() const;

private:
    std::string name_;
    std::vector<inviwo::Processor*> processors_;
    std::vector<AggregationMetaData*> aggregations_;
    std::vector<inviwo::PortConnection> portConnections_;
    int positionX_;
    int positionY_;
};

//-------------------------------------------------------------------------------------------------

class AggregationMetaDataContainer : public MetaDataBase {
public:
    AggregationMetaDataContainer();
    virtual ~AggregationMetaDataContainer();

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

    void addAggregation(AggregationMetaData* aggregation);
    void removeAggregation(AggregationMetaData* aggregation);
    void clearAggregations();
    bool isEmpty() const;

    const std::vector<AggregationMetaData*>& getAggregations() const;
    virtual MetaDataBase* clone() const;

protected:
    std::vector<AggregationMetaData*> aggregations_; ///< contains all stored aggregations and every processor within each aggregation
};

} // namespace

#endif // VRN_AGGREGATIONMETADATA_H

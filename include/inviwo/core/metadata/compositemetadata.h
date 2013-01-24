#ifndef IVW_COMPOSITE_METADATA_H
#define IVW_COMPOSITE_METADATA_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/metadata/metadata.h"

namespace inviwo {

class IVW_CORE_API CompositeMetaData : public MetaData {

public:
    CompositeMetaData();
    virtual ~CompositeMetaData();

    virtual std::string getClassName() const { return "CompositeMetaData"; }
    virtual CompositeMetaData* create() const;
    void addMetaData(MetaData* metaData);
    void addMetaData(MetaData& metaData);
    std::vector<MetaData*> getSubMetaData() { return subMetaData_; }
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    std::vector<MetaData*> subMetaData_;

protected:
    static const std::string logSource_; ///< Source string to be displayed for log messages.
    
};

} // namespace

#endif // IVW_COMPOSITE_METADATA_H
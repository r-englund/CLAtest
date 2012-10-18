#ifndef IVW_METADATA_H
#define IVW_METADATA_H

#include "inviwo/core/inviwo.h"


namespace inviwo {

class MetaData : public IvwSerializable {

public:
    MetaData();
    virtual ~MetaData();

    virtual std::string getClassName() const { return "MetaData"; }
    virtual MetaData* create() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:

    static const std::string logSource_; ///< Source string to be displayed for log messages.

};

} // namespace

#endif // IVW_PROCESSOR_H

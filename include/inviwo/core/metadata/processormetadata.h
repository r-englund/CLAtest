#ifndef IVW_PROCESSORMETADATA_H
#define IVW_PROCESSORMETADATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/compositemetadata.h>
#include <inviwo/core/metadata/positionmetadata.h>


namespace inviwo {

class IVW_CORE_API ProcessorMetaData : public CompositeMetaData {

public:
    ProcessorMetaData();
    virtual ~ProcessorMetaData();

    virtual std::string getClassName() const { return "ProcessorMetaData"; }
    virtual ProcessorMetaData* create() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    void setPosition(ivec2 pos);
    ivec2 getPosition();
    void setVisibile(bool visibility);
    bool isVisible();

private:
    IVec2MetaData positionMetaData_;
    BoolMetaData visiblityMetaData_;

protected:

    static const std::string logSource_; ///< Source string to be displayed for log messages.

};

} // namespace

#endif // IVW_PROCESSORMETADATA_H

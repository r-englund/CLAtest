#ifndef IVW_PROCESSOR_WIDGET_METADATA_H
#define IVW_PROCESSOR_WIDGET_METADATA_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/metadata/compositemetadata.h>
#include <inviwo/core/metadata/positionmetadata.h>


namespace inviwo {

class IVW_CORE_API ProcessorWidgetMetaData : public CompositeMetaData {

public:
    ProcessorWidgetMetaData();
    virtual ~ProcessorWidgetMetaData();

    virtual std::string getClassName() const { return "ProcessorWidgetMetaData"; }
    virtual ProcessorWidgetMetaData* create() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    void setWidgetPosition(ivec2 pos);
    ivec2 getWidgetPosition();
    void setDimension(ivec2 dim); 
    ivec2 getDimension() const;
    void setVisibile(bool visibility);
    bool isVisible() const;

private:
    IVec2MetaData positionMetaData_;
    IVec2MetaData dimensionMetaData_;
    BoolMetaData visiblityMetaData_;

};

} // namespace

#endif // IVW_PROCESSOR_WIDGET_METADATA_H

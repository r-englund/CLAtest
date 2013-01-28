#ifndef IVW_POSITION_METADATA_H
#define IVW_POSITION_METADATA_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/metadata/metadata.h>


namespace inviwo {

class IVW_CORE_API PositionMetaData : public IVec2MetaData {

public:
    PositionMetaData();
    PositionMetaData(int x, int y);
    virtual ~PositionMetaData();

    virtual std::string getClassName() const { return "PositionMetaData"; }
    virtual PositionMetaData* create() const;

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    ivec2 getXY();
    void setXY(const ivec2 &xy);

    int getX();
    void setX(const int &x);

    int getY();
    void setY(const int &y);

protected:

    static const std::string logSource_; ///< Source string to be displayed for log messages.

};

} // namespace

#endif // IVW_PROCESSOR_H

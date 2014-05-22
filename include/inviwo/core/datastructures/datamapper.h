#ifndef IVW_DATAMAPPER_H
#define IVW_DATAMAPPER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API DataMapper {
public:
    DataMapper();
    DataMapper(const DataFormatBase* format);
    virtual ~DataMapper() {}

    dvec2 dataRange;
    dvec2 valueRange;
    std::string valueUnit;

    void initWithFormat(const DataFormatBase* format);

    template <typename T>
    T mapFromDataToValue(T val) {
        return static_cast<T>((static_cast<double>(val) - dataRange.x) /
                                  (dataRange.y - dataRange.x) * (valueRange.y - valueRange.x) +
                              valueRange.x);
    }
    
    template <typename T>
    T mapFromValueToData(T val) {
        return static_cast<T>((static_cast<double>(val) - valueRange.x) /
                                  (valueRange.y - valueRange.x) * (dataRange.y - dataRange.x) +
                              dataRange.x);
    }
};

}  // namespace

#endif  // IVW_DATAMAPPER_H

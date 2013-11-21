#ifndef IVW_DATAREPRESENTATION_H
#define IVW_DATAREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/dataoperation.h>

namespace inviwo {

    class IVW_CORE_API DataRepresentation {

    public:
        DataRepresentation();
        DataRepresentation(const DataFormatBase* format);
        DataRepresentation(const DataRepresentation& rhs);
        virtual ~DataRepresentation();
        virtual void performOperation(DataOperation*) const = 0;

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;
        virtual DataRepresentation* clone() const = 0;
        virtual std::string getClassName() const;
        const DataFormatBase* getDataFormat() const;
        std::string getDataFormatString() const;
        DataFormatId getDataFormatId() const;

    protected:
        void setDataFormat(const DataFormatBase* format);

        const DataFormatBase* dataFormatBase_;
    };

} // namespace

#endif // IVW_DATAREPRESENTATION_H

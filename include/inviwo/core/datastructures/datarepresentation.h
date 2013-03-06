#ifndef IVW_DATAREPRESENTATION_H
#define IVW_DATAREPRESENTATION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/dataoperation.h>

namespace inviwo {

    class IVW_CORE_API DataRepresentation {

    public:
        DataRepresentation();
        virtual ~DataRepresentation();
        virtual void applyOperation(DataOperation*) = 0;

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;
        virtual DataRepresentation* clone() = 0;
        virtual std::string getClassName() const { return "DataRepresentation"; }
    };

} // namespace

#endif // IVW_DATAREPRESENTATION_H

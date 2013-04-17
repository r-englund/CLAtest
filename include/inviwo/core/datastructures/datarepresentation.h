#ifndef IVW_DATAREPRESENTATION_H
#define IVW_DATAREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/dataoperation.h>

namespace inviwo {

    class IVW_CORE_API DataRepresentation {

    public:
        DataRepresentation();
        virtual ~DataRepresentation();
        virtual void performOperation(DataOperation*) const = 0;

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;
        virtual DataRepresentation* clone() const = 0;
        virtual std::string getClassName() const;
        bool isValid() const;
        void invalidate();

    private:
        bool valid_;
    };

} // namespace

#endif // IVW_DATAREPRESENTATION_H

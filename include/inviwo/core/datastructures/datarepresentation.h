#ifndef IVW_DATAREPRESENTATION_H
#define IVW_DATAREPRESENTATION_H

#include "inviwo/core/inviwo.h"

namespace inviwo {

    class DataRepresentation {

    public:
        DataRepresentation();
        virtual ~DataRepresentation();

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;
        virtual DataRepresentation* clone() = 0;
        virtual std::string getClassName() const { return "DataRepresentation"; }
    };

} // namespace

#endif // IVW_DATAREPRESENTATION_H

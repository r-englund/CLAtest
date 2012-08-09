#ifndef IVW_DATAREPRESENTATION_H
#define IVW_DATAREPRESENTATION_H

namespace inviwo {

    class DataRepresentation {

    public:
        DataRepresentation();
        virtual ~DataRepresentation();

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;
    };

} // namespace

#endif // IVW_DATAREPRESENTATION_H

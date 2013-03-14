#ifndef IVW_IMAGERAM_H
#define IVW_IMAGERAM_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    class IVW_CORE_API ImageRAM : public ImageRepresentation {

    public:
        ImageRAM();
        ImageRAM(uvec2 dimension);
        ImageRAM(void* data, uvec2 dimension);
        virtual ~ImageRAM();
        virtual void initialize();
        void deinitialize();
        DataRepresentation* clone();
        virtual std::string getClassName() const { return "ImageRAM"; }
        void copyAndResizeImage(DataRepresentation*){}
        virtual void* getData() {return data_;}
    protected:
        void* data_;
    };

} // namespace

#endif // IVW_IMAGERAM_H

#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

    class IVW_CORE_API ImageRepresentation : public DataRepresentation {

    public:
        ImageRepresentation();
        ImageRepresentation(uvec2 dimensions);
        virtual ~ImageRepresentation();
        virtual void resize(uvec2 dimensions);
        virtual uvec2 getDimension() {return dimensions_;}
        virtual void copyAndResizeImage(DataRepresentation*)=0;
        virtual DataRepresentation* clone()=0;
        virtual std::string getClassName() const { return "ImageRepresentation"; }
   protected:
        uvec2 dimensions_;
   };

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H

#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

    class IVW_CORE_API ImageRepresentation : public DataRepresentation {

    public:
        ImageRepresentation();
        ImageRepresentation(uvec2 dimensions);
        ImageRepresentation(uvec2 dimensions, DataFormatBase format);
        virtual ~ImageRepresentation();
        virtual void performOperation(DataOperation*) const {};
        virtual void resize(uvec2 dimensions);
        virtual uvec2 getDimension() const {return dimensions_;}
        virtual bool copyAndResizeImage(DataRepresentation*)=0;
        virtual DataRepresentation* clone() const = 0;
        virtual std::string getClassName() const { return "ImageRepresentation"; }
   protected:
        uvec2 dimensions_;
   };

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H

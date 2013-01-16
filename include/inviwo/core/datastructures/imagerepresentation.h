#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/datastructures/datarepresentation.h"
#include "inviwo/core/inviwo.h"

namespace inviwo {

    class IVW_CORE_API ImageRepresentation : public DataRepresentation {

    public:
        ImageRepresentation();
        ImageRepresentation(ivec2 dimensions);
        virtual ~ImageRepresentation();
        virtual void resize(ivec2 dimensions);
        virtual ivec2 size() {return dimensions_;}
        virtual void copyAndResizeImage(DataRepresentation*)=0;
        virtual DataRepresentation* clone()=0;
        virtual std::string getClassName() const { return "ImageRepresentation"; }
   protected:
        ivec2 dimensions_;
   };

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H

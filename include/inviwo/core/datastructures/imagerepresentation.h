#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include "inviwo/core/datastructures/datarepresentation.h"
#include "inviwo/core/inviwo.h"

namespace inviwo {

    class ImageRepresentation : public DataRepresentation {

    public:
        ImageRepresentation();
        virtual ~ImageRepresentation();
        virtual void resize(ivec2 dimensions);
        virtual ivec2 size() {return ivec2(0,0);}
        virtual void copyAndResizeImage(DataRepresentation*)=0;
        virtual DataRepresentation* clone()=0;
        virtual std::string getClassName() const { return "ImageRepresentation"; }
        
    };

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H

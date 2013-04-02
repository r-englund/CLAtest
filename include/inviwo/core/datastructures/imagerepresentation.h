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
        ImageRepresentation(uvec2 dimensions, DataFormatBase format);
        virtual ~ImageRepresentation();
        virtual void applyOperation(DataOperation*) {};
        virtual void resize(uvec2 dimensions);
        virtual uvec2 getDimension() {return dimensions_;}
        virtual DataFormatBase getDataFormat() const {return dataFormatBase_;}
        virtual DataFormatId getDataFormatId() const {return dataFormatBase_.getId();}
        virtual void copyAndResizeImage(DataRepresentation*)=0;
        virtual DataRepresentation* clone()=0;
        virtual std::string getClassName() const { return "ImageRepresentation"; }
   protected:
        uvec2 dimensions_;
        DataFormatBase dataFormatBase_;
   };

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H

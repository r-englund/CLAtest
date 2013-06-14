#ifndef IVW_IMAGEDISK_H
#define IVW_IMAGEDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/diskrepresentation.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>

namespace inviwo {

    class IVW_CORE_API ImageDisk : public ImageRepresentation, public DiskRepresentation {

    public:
        ImageDisk();
		ImageDisk(std::string url);
        virtual ~ImageDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone() const;
        virtual std::string getClassName() const { return "ImageDisk"; };
        void copyAndResizeImage(DataRepresentation*){};
        void* loadFileData() const;
    };

} // namespace

#endif // IVW_IMAGEDISK_H

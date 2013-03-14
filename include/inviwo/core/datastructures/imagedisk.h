#ifndef IVW_IMAGEDISK_H
#define IVW_IMAGEDISK_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    class IVW_CORE_API ImageDisk : public ImageRepresentation {

    public:
        ImageDisk();
		ImageDisk(std::string url);
        virtual ~ImageDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone();
        virtual std::string getClassName() const { return "ImageDisk"; }
        void copyAndResizeImage(DataRepresentation*){}
        void* ImageDisk::loadFileData();
	private:
		std::string sourceFile_;
    };

} // namespace

#endif // IVW_IMAGEDISK_H

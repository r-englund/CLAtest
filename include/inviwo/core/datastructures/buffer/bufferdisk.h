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
        virtual void resize(uvec2 dimensions);
        virtual DataRepresentation* clone() const;
        virtual std::string getClassName() const { return "ImageDisk"; };
        virtual bool copyAndResizeImage(DataRepresentation*){ return false;};
        /** 
         * \brief loads data from url.
         *
         * @return void* return the raw data
         */
        void* loadFileData() const;
        /** 
         * \brief loads and rescales data from url.
         * 
         * @param uvec2 dst_dimesion destination dimension
         * @return void* returns the raw data that has been rescaled to dst_dimension
         */
        void* loadFileDataAndRescale(uvec2 dst_dimesion) const;
    };

} // namespace

#endif // IVW_IMAGEDISK_H

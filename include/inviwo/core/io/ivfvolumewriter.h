#ifndef IVW_IVFVOLUMEWRITER_H
#define IVW_IVFVOLUMEWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

    class IVW_CORE_API IvfVolumeWriter {

    public:
        IvfVolumeWriter();
        virtual ~IvfVolumeWriter();

        std::string getIdentifier() const;

    protected:

        void setIdentifier(const std::string& name);

    private:
        std::string identifier_;
    };

} // namespace

#endif // IVW_IVFVOLUMEWRITER_H

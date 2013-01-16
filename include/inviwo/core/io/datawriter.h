#ifndef IVW_DATAWRITER_H
#define IVW_DATAWRITER_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"

namespace inviwo {

    class IVW_CORE_API DataWriter {

    public:
        DataWriter();
        virtual ~DataWriter();

        std::string getIdentifier() const;

    protected:

        void setIdentifier(const std::string& name);

    private:
        std::string identifier_;
    };

} // namespace

#endif // IVW_DATAWRITER_H

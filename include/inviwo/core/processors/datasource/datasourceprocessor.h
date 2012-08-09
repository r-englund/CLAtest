#ifndef IVW_DATASOURCEPROCESSOR_H
#define IVW_DATASOURCEPROCESSOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

    class DataSourceProcessor : public Processor {
    public:
        DataSourceProcessor();

        virtual void initialize();
        virtual void deinitialize();

    protected:
        virtual void process();
    };

} // namespace

#endif // IVW_DATASOURCEPROCESSOR_H

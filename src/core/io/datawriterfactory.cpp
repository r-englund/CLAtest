#include <inviwo/core/io/datawriterfactory.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <inviwo/core/io/datawriter.h>

namespace inviwo {

DataWriterFactory::DataWriterFactory() {
}

void DataWriterFactory::registerObject(DataWriter* writer) {
    for (std::vector<FileExtension>::const_iterator it = writer->getExtensions().begin();
         it != writer->getExtensions().end(); ++it) {
        if (writerForExtension_.find(it->extension_) == writerForExtension_.end())
            writerForExtension_.insert(std::make_pair(it->extension_, writer));
    }
}

} // namespace


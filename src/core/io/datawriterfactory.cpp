#include <inviwo/core/io/datawriterfactory.h>

namespace inviwo {

DataWriterFactory::DataWriterFactory(){
    initialize();
}

void DataWriterFactory::initialize() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t module=0; module<inviwoApp->getModules().size(); module++) {
        std::vector<DataWriter*> writers = inviwoApp->getModules()[module]->getDataWriters();
        for (size_t i=0; i<writers.size(); i++)
            registerDataWriter(writers[i]);
    }
}

void DataWriterFactory::registerDataWriter( DataWriter* writer ){
    for( std::vector<FileExtension>::const_iterator it = writer->getExtensions().begin();
        it != writer->getExtensions().end(); ++it){
            if(writerForExtension_.find(it->extension_) == writerForExtension_.end()){
                writerForExtension_.insert(std::make_pair(it->extension_, writer));
            }
    }
}


void DataWriterFactory::deinitialize(){

}




} // namespace


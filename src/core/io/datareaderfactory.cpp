#include <inviwo/core/io/datareaderfactory.h>

namespace inviwo {

    DataReaderFactory::DataReaderFactory(){
        initialize();
    }

    void DataReaderFactory::initialize() {
        //TODO: check that inviwoapp is initialized

        InviwoApplication* inviwoApp = InviwoApplication::getPtr();
        for (size_t module=0; module<inviwoApp->getModules().size(); module++) {
            std::vector<DataReader*> readers = inviwoApp->getModules()[module]->getDataReaders();
            for (size_t i=0; i<readers.size(); i++)
                registerDataReader(readers[i]);
        }
    }

    void DataReaderFactory::registerDataReader( DataReader* reader ){
        for( std::vector<FileExtension>::const_iterator it = reader->getExtensions().cbegin();
            it != reader->getExtensions().cend(); ++it){
             if(readerForExtension_.find(it->extension_) == readerForExtension_.end()){
                 readerForExtension_.insert(std::make_pair(it->extension_, reader));
             }
        }
    }


    void DataReaderFactory::deinitialize(){
        throw std::exception("The method or operation is not implemented.");
    }






} // namespace


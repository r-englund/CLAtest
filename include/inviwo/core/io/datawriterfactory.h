#ifndef IVW_DATAWRITERFACTORY_H
#define IVW_DATAWRITERFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API DataWriterFactory : public Factory, public Singleton<DataWriterFactory> { 
public:
    DataWriterFactory();
    virtual ~DataWriterFactory(){}

    virtual void initialize();
    virtual void deinitialize();

    void registerDataWriter( DataWriter* reader );

    template <typename T>
    std::vector<FileExtension> getExtensionsForType(){
        std::vector<FileExtension> ext;

        for(ExtensionMap::const_iterator it = writerForExtension_.begin();
            it != writerForExtension_.end(); ++it){
                DataWriterType<T>* r = dynamic_cast<DataWriterType<T>* >(it->second);
                if(r){
                    std::vector<FileExtension> writerExt = r->getExtensions();
                    for(std::vector<FileExtension>::const_iterator e = writerExt.begin();
                        e != writerExt.end(); ++e){
                            ext.push_back(*e);
                    }
                }

        }        
        return ext;
    }

    template <typename T>
    DataWriterType<T>* getWriterForTypeAndExtension(std::string ext){
        ExtensionMap::iterator it = writerForExtension_.find(ext);
        if (it != writerForExtension_.end()){
            DataWriterType<T>* r = dynamic_cast<DataWriterType<T>* >(it->second);
            if(r){
                return r->clone();
            }
        }
        return NULL;
    }

    typedef std::map<std::string, DataWriter*> ExtensionMap;

private:
    ExtensionMap writerForExtension_;


};

} // namespace

#endif // IVW_DATAWRITERFACTORY_H


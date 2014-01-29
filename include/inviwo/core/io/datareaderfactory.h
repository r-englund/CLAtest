/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_DATAREADERFACTORY_H
#define IVW_DATAREADERFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>

#include <inviwo/core/io/datareader.h>

namespace inviwo {

class IVW_CORE_API DataReaderFactory : public Factory, public Singleton<DataReaderFactory> { 
public:
    DataReaderFactory();
    virtual ~DataReaderFactory(){}



    void registerObject( DataReader* reader );

    template <typename T>
    std::vector<FileExtension> getExtensionsForType(){
        std::vector<FileExtension> ext;

        for(ExtensionMap::const_iterator it = readerForExtension_.begin();
            it != readerForExtension_.end(); ++it){
                DataReaderType<T>* r = dynamic_cast<DataReaderType<T>* >(it->second);
                if(r){
                    std::vector<FileExtension> readerExt = r->getExtensions();
                    for(std::vector<FileExtension>::const_iterator e = readerExt.begin();
                        e != readerExt.end(); ++e){
                            ext.push_back(*e);
                    }
                }

        }        
        return ext;
    }

    template <typename T>
    DataReaderType<T>* getReaderForTypeAndExtension(std::string ext){
        ExtensionMap::iterator it = readerForExtension_.find(ext);
        if (it != readerForExtension_.end()){
            DataReaderType<T>* r = dynamic_cast<DataReaderType<T>* >(it->second);
            if(r){
                return r->clone();
            }
        }
        return NULL;
    }

    typedef std::map<std::string, DataReader*> ExtensionMap;

private:
    ExtensionMap readerForExtension_;

};

} // namespace

#endif // IVW_DATAREADERFACTORY_H


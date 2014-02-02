/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_DATAREADERDIALOGFACTORY_H
#define IVW_DATAREADERDIALOGFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API DataReaderDialogFactory : public Singleton<DataReaderDialogFactory>  {

public:
    DataReaderDialogFactory();
    virtual ~DataReaderDialogFactory();

    void registerObject(DataReaderDialog* dataReaderDialog);
    
    template <typename T>
    DataReaderDialog* getDataReaderDialog() {
        for (std::map<DataReader*, DataReaderDialog*>::const_iterator it=dataReaderDialogs_.begin(); it!=dataReaderDialogs_.end(); ++it)
            if (dynamic_cast<T*>(it->first))
                return it->second;
        /*
        for (size_t i=0; i<dataReaderDialogs_.size(); i++) {
            DataReaderType<T>* dataReaderType = dynamic_cast<DataReaderType<T>* >(it->first);
            if (dataReaderType)
                return it->second;
        }
        */
        return 0;
    }

    template <typename T>
    void registerDataReaderDialog(T* dataReader, DataReaderDialog* dataReaderDialog) {
        for (std::map<DataReader*, DataReaderDialog*>::const_iterator it=dataReaderDialogs_.begin(); it!=dataReaderDialogs_.end(); ++it)
            if (dynamic_cast<T*>(it->first)) {
                LogWarn("DataReaderDialog already registered.");
                return;
            }
        dataReaderDialogs_.insert(std::make_pair(dataReader, dataReaderDialog));
    }

private:
    std::map<DataReader*, DataReaderDialog*> dataReaderDialogs_;
};

} // namespace

#endif // IVW_DATAREADERDIALOGFACTORY_H

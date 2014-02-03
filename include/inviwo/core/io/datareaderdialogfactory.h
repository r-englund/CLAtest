 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_DATAREADERDIALOGFACTORY_H
#define IVW_DATAREADERDIALOGFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/io/datareader.h>
#include <inviwo/core/io/datareaderdialog.h>
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

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
 *********************************************************************************/

#ifndef IVW_PYTHONEXECUTIONOUTPUTOBSERVABLE_H
#define IVW_PYTHONEXECUTIONOUTPUTOBSERVABLE_H

#include <modules/python3/python3moduledefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/util/observer.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {
    enum IVW_MODULE_PYTHON3_API PythonExecutionOutputStream
    {
        sysstdout,
        sysstderr
    };

    class IVW_MODULE_PYTHON3_API PythonExecutionOutputObeserver : public Observer {
    public:

        virtual void onPyhonExecutionOutput(const std::string &msg, const PythonExecutionOutputStream &outputType) = 0;
    };

    class IVW_MODULE_PYTHON3_API PythonExecutionOutputObservable
        : public Observable<PythonExecutionOutputObeserver>
        , public Singleton <PythonExecutionOutputObservable>
    {
    public:
        void pythonExecutionOutputEvent(const std::string &msg, const PythonExecutionOutputStream &outputType);
    };


} // namespace

#endif // IVW_PYTHONEXECUTIONOUTPUTOBSERVABLE_H


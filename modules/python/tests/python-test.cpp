/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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

#include <gtest/gtest.h>


#include <modules/unittests/unittestsmoduledefine.h>

#include <modules/python/pythonmodule.h>
#include <modules/python/pythonscript.h>

namespace inviwo{

class IVW_MODULE_UNITTESTS_API PythonModuleTest : public ::testing::Test {
protected:
    PythonModuleTest():module_(0) {}
    ~PythonModuleTest() {}

    virtual void SetUp() {
        const std::vector<InviwoModule*> modules = InviwoApplication::getPtr()->getModules();

        for (size_t i = 0; i<modules.size(); i++) {
            const PythonModule* pyModule = dynamic_cast<const PythonModule*>(modules[i]);

            if (pyModule) {
                module_ = const_cast<PythonModule*>(pyModule);
            }
        }
    }
    virtual void TearDown() {}

    PythonModule* module_;


};

TEST_F(PythonModuleTest, init) {
    ASSERT_TRUE(module_!=0);
}




}
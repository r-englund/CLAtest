#ifndef IVW_PYTHON_TEST_H
#define IVW_PYTHON_TEST_H

#include <modules/unittests/unittestsmoduledefine.h>

#include <modules/python/pythonmodule.h>
#include <modules/python/pythonscript.h>

class IVW_MODULE_UNITTESTS_API PythonModuleTest : public ::testing::Test {
protected:
    PythonModuleTest():module_(0){}
    ~PythonModuleTest(){}
    
    virtual void SetUp(){
        const std::vector<InviwoModule*> modules = InviwoApplication::getPtr()->getModules();
        for(size_t i = 0;i<modules.size();i++){
            const PythonModule* pyModule = dynamic_cast<const PythonModule*>(modules[i]);
            if(pyModule){
                module_ = const_cast<PythonModule*>(pyModule);
            }
        }
    }
    virtual void TearDown(){}

    PythonModule* module_;


};

TEST_F (PythonModuleTest, init){
    ASSERT_TRUE(module_!=0);
}




#endif
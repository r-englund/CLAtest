#include "kernelmanager.h"
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/vectoroperations.h>
#include <modules/opengl/openglmodule.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

KernelManager::KernelManager() {
    InviwoApplication::getRef().registerFileObserver(this);
}

KernelManager::~KernelManager()
{
    for(ProgramMap::iterator programIt = programs_.begin(); programIt != programs_.end(); ++programIt) {
        InviwoApplication::getRef().stopFileObservation(programIt->first);
    }
    for(KernelMap::iterator kernelIt = kernels_.begin(); kernelIt != kernels_.end(); ++kernelIt) {
        delete kernelIt->second; 
        kernelIt->second = NULL;
    }
    for(ProgramMap::iterator programIt = programs_.begin(); programIt != programs_.end(); ++programIt) {
        delete programIt->second.program;
        programIt->second.program = NULL;
    }
}

cl::Program* KernelManager::buildProgram( const std::string& fileName, const std::string& defines /*= ""*/ )
{
    std::pair <ProgramMap::iterator, ProgramMap::iterator> range = programs_.equal_range(fileName);
    for(ProgramMap::iterator it = range.first; it != range.second; ++it) {
        if(it->second.defines == defines) {
            return it->second.program;
        }
    }
    cl::Program *program = new cl::Program();
    try {
        *program = cl::Program(OpenCL::buildProgram(fileName, defines));
    } catch (cl::Error&) {

    }
    ProgramIdentifier uniqueProgram; uniqueProgram.defines = defines; uniqueProgram.program = program;
    programs_.insert(std::pair<std::string, ProgramIdentifier>(fileName, uniqueProgram));
    startFileObservation(fileName);
    return program;
}

cl::Kernel* KernelManager::getKernel( cl::Program* program, const std::string& kernelName )
{
    // TODO: Make sure that this works even if the kernel does not exist
    cl::Kernel* kernel = new cl::Kernel(*program, kernelName.c_str());
    kernels_.insert(std::pair<cl::Program*, cl::Kernel*>(program, kernel));
    return kernel;
}

void KernelManager::fileChanged( std::string fileName )
{
    std::pair <ProgramMap::iterator, ProgramMap::iterator> programRange = programs_.equal_range(fileName);
    for(ProgramMap::iterator programIt = programRange.first; programIt != programRange.second; ++programIt) {
        cl::Program* program = programIt->second.program;   
        std::pair <KernelMap::iterator, KernelMap::iterator> range = kernels_.equal_range(program);
        std::vector<std::string> kernelNames;
        for(KernelMap::iterator kernelIt = range.first; kernelIt != range.second; ++kernelIt) {
            std::string kernelName;
            kernelIt->second->getInfo(CL_KERNEL_FUNCTION_NAME, &kernelName);
            kernelNames.push_back(kernelName);
            //cl::ReferenceHandler<cl::Kernel>::release(*it->second);
        }

        try {
            *program = OpenCL::buildProgram(fileName, programIt->second.defines);
            int i = 0;
            for(KernelMap::iterator kernelIt = range.first; kernelIt != range.second; ++kernelIt, ++i) {
                *(kernelIt->second) = cl::Kernel(*kernelIt->first, kernelNames[i].c_str());
            }
            LogInfo(fileName + " successfuly reloaded");
            InviwoApplication::getRef().playSound(InviwoApplication::IVW_OK);
            std::vector<Processor*> processors = InviwoApplication::getRef().getProcessorNetwork()->getProcessors();
            for (size_t i=0;i<processors.size();i++)
                processors[i]->invalidate(PropertyOwner::INVALID_RESOURCES);
        } catch (cl::Error&) {
            InviwoApplication::getRef().playSound(InviwoApplication::IVW_ERROR);
        }
    }
}




} // namespace

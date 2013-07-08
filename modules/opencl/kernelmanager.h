#ifndef IVW_KERNEL_MANAGER_H
#define IVW_KERNEL_MANAGER_H

#include <modules/opencl/openclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/util/singleton.h>
#include <modules/opencl/inviwoopencl.h>
#include <map>

namespace inviwo {

class IVW_MODULE_OPENCL_API KernelManager : public Singleton<KernelManager>, public FileObserver {
/** \class KernelManager 
*
* Manages building of OpenCL programs and kernels. 
* Reloads and builds program when file changes. Notifies processor when the program has been rebuilt.
*/
// TODO: Parallel building of program.
// TODO: Make sure that processor is not evaluated while building or build failed.
public:
    // Multiple programs can be created from the same file but with different defines
    struct ProgramIdentifier {
        cl::Program* program;
        std::string defines;
    };
    typedef std::multimap<std::string, ProgramIdentifier> ProgramMap; ///< File name and unique identifier for program
    typedef std::multimap<cl::Program*, cl::Kernel*> KernelMap;
    //typedef std::map<cl::Program*, std::vector<cl::Kernel>> KernelMap;

    KernelManager();
    virtual ~KernelManager();
    /**
     * Creates and builds an OpenCL program. Will automatically reload the program and
     * kernels when the file changes.
     * @note KernelManager manages pointer memory, do not delete it.
     *
     * @param fileName (const std::string &) File name
     * @param defines (const std::string &) Compiler defines
     * @return (cl::Program*) Pointer to a program no matter if it was succesfully built or not. 
     * The KernelManager will make sure that the processor is not evaluated if building fails.
     */
    cl::Program* buildProgram(const std::string& fileName, const std::string& defines = "");;


    /**
     * Creates a kernel from a previously created cl::Program. 
     * Makes sure that it is up to date when program is rebuilt.
     * 
     * @see cl::Program, cl::Kernel
     * @param program (cl::Program *)
     * @param kernelName (const std::string &)
     * @return (cl::Kernel*) Pointer to cl::Kernel. KernelManager manages memory of Kernel, do not delete it.
     */
    cl::Kernel* getKernel(cl::Program* program, const std::string& kernelName);;

    /**
     * Reloads programs from file and notifies processors.
     * 
     */
    virtual void fileChanged(std::string fileName);;

private:
    ProgramMap programs_;
    KernelMap kernels_;
};

} // namespace

#endif // IVW_KERNEL_MANAGER_H

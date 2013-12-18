#ifndef IVW_UNITTESTS_MODULE_H
#define IVW_UNITTESTS_MODULE_H

#include <modules/unittests/unittestsmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_MODULE_UNITTESTS_API UnitTestsModule : public InviwoModule {
public:
	UnitTestsModule();
	virtual ~UnitTestsModule();
	virtual void initialize();
	virtual void deinitialize();
    static int runAllTests();
protected:
};
} // namespace




#endif // IVW_UNITTESTS_MODULE_H

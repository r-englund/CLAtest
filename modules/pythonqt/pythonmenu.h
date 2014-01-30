#ifndef IVW_PYTHONMENU_H
#define IVW_PYTHONMENU_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {
    class PythonInfoWidget;
    class PythonEditorWidget;
class IVW_MODULE_PYTHONQT_API PythonMenu { 
public:
    PythonMenu();
    virtual ~PythonMenu();

private:
    PythonInfoWidget* infoWidget_;
    PythonEditorWidget* pythonEditorWidget_;
};

} // namespace

#endif // IVW_PYTHONMENU_H


#ifndef IVW_PYTHONEDITORYWIDGET_H
#define IVW_PYTHONEDITORYWIDGET_H

#include <modules/python/pythonmoduledefine.h>
#include <inviwo/qt/editor/inviwodockwidget.h>

#include <apps/inviwo/inviwomainwindow.h>

#include <inviwo/core/util/fileobserver.h>

#include <modules/python/pythonscript.h>

namespace inviwo{
    
class IVW_MODULE_PYTHON_API PythonEditorWidget : public InviwoDockWidget , public FileObserver, public inviwo::Singleton<PythonEditorWidget> {
    Q_OBJECT;

	PythonEditorWidget(InviwoMainWindow* mainWindow);
public:
    virtual ~PythonEditorWidget();

	static void init(InviwoMainWindow* mainWindow);

    void appendToOutput(const std::string &msg);
    virtual void fileChanged(std::string fileName);
    void loadFile(std::string fileName,bool askForSave = true);
private:
    void buildWidget();

    QTextEdit* pythonCode_;
    QTextEdit* pythonOutput_;

    PythonScript script_;
    std::string scriptFileName_;

    bool unsavedChanges_;
    void readFile();

public slots:
    void save();
    void saveAs();
    void open();
    void run();
    void show();
    void setDefaultText();
    void clearOutput();
    void onTextChange();
};

}//namespace

#endif //IVW_PYTHONEDITORYWIDGET_H
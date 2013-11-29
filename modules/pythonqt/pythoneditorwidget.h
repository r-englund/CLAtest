#ifndef IVW_PYTHONEDITORYWIDGET_H
#define IVW_PYTHONEDITORYWIDGET_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <modules/python/pythonscript.h>
#include <modules/python/pythonmodule.h>
#include <modules/python/pythonexecutionoutputobeserver.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <QTextEdit>
#include <QColor>

namespace inviwo{
    
class IVW_MODULE_PYTHONQT_API PythonEditorWidget : public InviwoDockWidget , public FileObserver , public PythonExecutionOutputObeserver {
    Q_OBJECT

public:
    PythonEditorWidget(QWidget* parent = NULL);
    virtual ~PythonEditorWidget();

    void appendToOutput(const std::string &msg,bool error = false);
    virtual void fileChanged(std::string fileName);
    void loadFile(std::string fileName,bool askForSave = true);

	virtual void onPyhonExecutionOutput(std::string msg,OutputType outputType);

private:
    void buildWidget();

    QTextEdit* pythonCode_;
    QTextEdit* pythonOutput_;

	QColor infoTextColor_;
	QColor errorTextColor_;

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
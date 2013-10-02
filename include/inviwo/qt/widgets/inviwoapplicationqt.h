#ifndef IVW_INVIWOAPPLICATIONQT_H
#define IVW_INVIWOAPPLICATIONQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QApplication>
#include <QMainWindow>
#include <QFileSystemWatcher>

#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

class IVW_QTWIDGETS_API InviwoApplicationQt : public QApplication, public InviwoApplication {

    Q_OBJECT;

public:
    InviwoApplicationQt(std::string displayName_, std::string basePath_,
                        int& argc, char** argv);

    void setMainWindow(QMainWindow* mainWindow) { mainWindow_ = mainWindow; }
    QMainWindow* getMainWindow() { return mainWindow_; }

    virtual void registerFileObserver(FileObserver* fileObserver);
    virtual void startFileObservation(std::string fileName);
    virtual void stopFileObservation(std::string fileName);

    virtual void playSound(unsigned int soundID);

	virtual void initialize();

public slots:
    void fileChanged(QString fileName);

protected:
    void wait(int);

private:
    QMainWindow* mainWindow_;
    std::vector<FileObserver*> fileObservers_;
    QFileSystemWatcher* fileWatcher_;
};

} // namespace

#endif // IVW_INVIWOAPPLICATIONQT_H

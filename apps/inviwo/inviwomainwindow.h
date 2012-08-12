#ifndef IVW_INVIWOMAINWINDOW_H
#define IVW_INVIWOMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>

namespace inviwo {

    class InviwoMainWindow : public QMainWindow {
        Q_OBJECT

    public:
        InviwoMainWindow();
        ~InviwoMainWindow();

};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H

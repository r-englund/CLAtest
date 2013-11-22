#ifndef IVW_PROPERTYWIDGETQT_H
#define IVW_PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMenu>

#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

    class IVW_QTWIDGETS_API PropertyWidgetQt : public QWidget, public PropertyWidget, public VoidObserver {

        Q_OBJECT

    public:
        PropertyWidgetQt();
        PropertyWidgetQt* create();

        void showWidget();
        void hideWidget();
        //void notify();
        void updateContextMenu();
        public slots:
            void visibilityModified(int mode);
            void showContextMenu(const QPoint& pos);
            void setDeveloperViewMode(bool value);
            void setApplicationViewMode(bool value);
            void addToStudy(bool value);

signals:
            void modified();
            void visibilityChange();
    private:
        QMenu* contextMenu_;
        QActionGroup* viewModeActionGroup_;
        QMenu* viewModeItem_;
        Property* property_;
    protected:
        QMenu* getContextMenu(){return contextMenu_;}
        void generateContextMenu();
        QMenu* generatePropertyWidgetMenu();
        QAction* developerViewModeAction_;
        QAction* applicationViewModeAction_;
        QAction* addToStudyAction_;
        void setProperty(Property* prop);


    };

} // namespace

#endif // IVW_PROPERTYWIDGETQT_H
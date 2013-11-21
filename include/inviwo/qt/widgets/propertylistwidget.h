 #ifndef IVW_PROPERTYLISTWIDGET_H
#define IVW_PROPERTYLISTWIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/processors/processor.h>

#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QCheckBox>
#include <QScrollArea>

namespace inviwo {

    class IVW_QTWIDGETS_API PropertyListWidget : public InviwoDockWidget, public VoidObservable {

        Q_OBJECT

    public:

        static PropertyListWidget* instance();

        PropertyListWidget(QWidget* parent);
        ~PropertyListWidget();

        void showProcessorProperties(Processor* processor);
        void removeProcessorProperties(Processor* processor);
        void showProcessorProperties(std::vector<Processor*> processors);
        void removeAllProcessorProperties();
        void saveState();
        void notify();
        void setMultiSelect(bool multiSelect){multiSelect_ = multiSelect; };
        PropertyVisibility::VisibilityMode getVisibilityMode();

        public slots:
            void setDeveloperViewMode(bool value);
            void setApplicationViewMode(bool value);

            protected slots:
                void propertyModified();

    private:
        QWidget* createNewProcessorPropertiesItem(Processor* processor);
        void addProcessorPropertiesToLayout(Processor* processor);
 

        bool developerViewMode_;
        bool applicationViewMode_;
        bool multiSelect_;

        QVBoxLayout* listWidgetLayout_;
        QWidget* listWidget_;
        QScrollArea* scrollArea_;

        std::vector<Property*> properties_;

    protected:
        static PropertyListWidget* propertyListWidget_;
        mutable std::map<std::string, QWidget*> propertyWidgetMap_;
    };

} // namespace

#endif // IVW_PROPERTYLISTWIDGET_H
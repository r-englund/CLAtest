#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>

namespace inviwo {

    PropertySettingsWidgetQt::PropertySettingsWidgetQt(FloatProperty *property) : property_(property), btnPropertyApply_("Apply", "Apply"), btnPropertyCancel_("Cancel", "Cancel"), btnPropertyReload_("Reload","Reload") {
        generateWidget();
    }


    void PropertySettingsWidgetQt::generateWidget() {

        btnApply_ = new ButtonPropertyWidgetQt(&btnPropertyApply_);
        btnCancel_ = new ButtonPropertyWidgetQt(&btnPropertyCancel_);
        btnReload_ = new ButtonPropertyWidgetQt(&btnPropertyReload_);
        btnPropertyApply_.registerClassMemberFunction(this, &PropertySettingsWidgetQt::save);
        btnPropertyCancel_.registerClassMemberFunction(this,&PropertySettingsWidgetQt::cancel);
        btnPropertyReload_.registerClassMemberFunction(this,&PropertySettingsWidgetQt::reload);


        QGridLayout* gridLayout = new QGridLayout();
        lineEditMax_ = new QLineEdit();
        lineEditMin_ = new QLineEdit();

        lineEditMin_->setText(QString::number(property_->getMinValue()));
        lineEditMax_->setText(QString::number(property_->getMaxValue()));


        gridLayout->addWidget(new QLabel("Min value"),1,1);
        gridLayout->addWidget(lineEditMin_,1,2);
        gridLayout->addWidget(new QLabel("Max value"),2,1);
        gridLayout->addWidget(lineEditMax_,2,2);
        gridLayout->addWidget(btnApply_,3,1);
        gridLayout->addWidget(btnCancel_,3,2);
        gridLayout->addWidget(btnReload_,3,3);

        setLayout(gridLayout);

        setWindowTitle(QString::fromStdString(property_->getDisplayName()));

    }

    void PropertySettingsWidgetQt::cancel() {
        hide();
        reload();
    }

    void PropertySettingsWidgetQt::reload() {

        lineEditMin_->setText(QString::number(property_->getMinValue()));
        lineEditMax_->setText(QString::number(property_->getMaxValue()));
    }

    void PropertySettingsWidgetQt::save() {
        
        property_->setMaxValue(lineEditMax_->text().toFloat());
        property_->setMinValue(lineEditMin_->text().toFloat());

    }

}//namespace
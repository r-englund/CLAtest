#include <inviwo/qt/widgets/properties/visibilitywidgetqt.h>
#include <QComboBox>
#include <typeinfo>
namespace inviwo {
    VisibilityWidgetQt::VisibilityWidgetQt(BaseOptionProperty* property) : property_(property) { 
        generateWidget();
        updateFromProperty();
    }

    void VisibilityWidgetQt::generateWidget() {
        QHBoxLayout* hLayout = new QHBoxLayout();
        comboBox_ = new CustomComboBoxQt();
        comboBox_->setFixedWidth(15);
        comboBox_->setStyleSheet("QComboBox{padding: 2 -19px 2 1; width: 0px;} QComboBox::drop-down{border-width: 0px;} QComboBox::down-arrow {image: url(noArrow); border-width: 0px;}");
        fillComboBox();
        updateFromProperty();
        hLayout->addWidget(comboBox_);
        hLayout->setContentsMargins(0,0,0,0);
        hLayout->setMargin(0);
        setLayout(hLayout);
        connect(comboBox_, SIGNAL(currentIndexChanged(int)),this, SLOT(optionChanged()));
    }

    void VisibilityWidgetQt::fillComboBox() {
        for (int i=0; i<property_->numOptions(); i++) {
            comboBox_->addItem(QString::fromStdString(property_->getOptionDisplayNames()[i]));
        }
    }
    void VisibilityWidgetQt::optionChanged() {
        property_->setSelectedOption(comboBox_->currentIndex());
        emit modified();
        emit optionChanged(property_->getSelectedOption());
    }

    void VisibilityWidgetQt::updateFromProperty() {
        int index = property_->getSelectedOption();
        comboBox_->setCurrentIndex(index);
    }

} // namespace

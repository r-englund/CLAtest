#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

namespace inviwo {

PropertyWidgetQt::PropertyWidgetQt() {
}

void PropertyWidgetQt::showWidget() {
    this->show();
    emit visibilityChange();
}
void PropertyWidgetQt::hideWidget(){
    this->hide();
    emit visibilityChange();
}

void PropertyWidgetQt::visibilityModified( int mode ){}


} // namespace

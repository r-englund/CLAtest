#ifndef IVW_CUSTOMCOMBOBOXQT_H
#define IVW_CUSTOMCOMBOBOXQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QComboBox>
#include <QApplication>
#include <QAbstractItemView>

namespace inviwo {

class IVW_QTWIDGETS_API CustomComboBoxQt : public QComboBox {
    public:

        void showPopup(){
            view()->setFixedWidth(70);
            QComboBox::showPopup();
        };
};
}//namespace
#endif //IVW_CUSTOMSLIDERWIDGETQT_H
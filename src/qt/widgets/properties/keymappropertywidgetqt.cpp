#include <inviwo/qt/widgets/properties/keymappropertywidgetqt.h>

namespace inviwo {

KeyMapPropertyWidgetQt::KeyMapPropertyWidgetQt( KeyMapProperty* property ) : property_(property)
{
    
    generateWidget();
}

void KeyMapPropertyWidgetQt::generateWidget()
{
    window_ = new QWidget();
    window_->setWindowFlags(Qt::WindowStaysOnTopHint);
    window_->setWindowTitle("Change key bindings");

    //keymapper_ = property_->getMapper();

    QVBoxLayout *leftVbox = new QVBoxLayout();
    QVBoxLayout *rightVbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout(); 

   
    QPushButton *buttons[3];
    QLabel *labels[3];

    for (int i = 0; i < 3; i++) {         
        buttons[i] = new QPushButton("Button " + intToQString(i));
        labels[i] = new QLabel("Label " + intToQString(i));
        rightVbox->addWidget(buttons[i]);
        leftVbox->addWidget(labels[i]);
    }
    
    hbox->addLayout(leftVbox);
    hbox->addLayout(rightVbox);

    //window_->setLayout(hbox);
    setLayout(hbox);

}

void KeyMapPropertyWidgetQt::updateFromProperty()
{
    if (!window_->isVisible()){
        window_->show();        
    } else {
        window_->hide();
    }
}


void KeyMapPropertyWidgetQt::setKeyMapper( TrackballKeyMapper *keymapper )
{
    keymapper_ = keymapper;
}

QString KeyMapPropertyWidgetQt::intToQString( int num )
{
    std::stringstream convert;
    convert << num;
    return convert.str().c_str();
}

} // namespace
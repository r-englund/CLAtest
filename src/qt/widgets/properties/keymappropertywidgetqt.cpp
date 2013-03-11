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
    
    keymapper_ = property_->getMapper();

    QVBoxLayout *leftVbox = new QVBoxLayout();
    QVBoxLayout *rightVbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout(); 

    QPushButton *buttons[TrackballKeyMapper::COUNT];
    QLabel *labels[TrackballKeyMapper::COUNT];

    for (int i = 0; i < TrackballKeyMapper::COUNT; i++)
    {
        buttons[i] = new QPushButton(intToQString(keymapper_->getKey(i)));
        labels[i] = new QLabel(keymapper_->getActionName(i).c_str());
        rightVbox->addWidget(buttons[i]);
        leftVbox->addWidget(labels[i]);
    }

    hbox->addLayout(leftVbox);
    hbox->addLayout(rightVbox);

    window_->setLayout(hbox);

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
#include <inviwo/qt/widgets/properties/keymappropertywidgetqt.h>
#include <QPushButton>

namespace inviwo {

KeyMapPropertyWidgetQt::KeyMapPropertyWidgetQt( Property* property ) : property_(property)
{
    generateWidget();
}

void KeyMapPropertyWidgetQt::generateWidget()
{
    window_ = new QWidget();
    window_->setWindowFlags(Qt::WindowStaysOnTopHint);
    window_->setWindowTitle("Change key bindings");   

    QVBoxLayout *leftVbox = new QVBoxLayout();
    QVBoxLayout *rightVbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout(); 
    QPushButton *button1 = new QPushButton("Button 1");    
    QPushButton *button2 = new QPushButton("Button 2");
    QPushButton *button3 = new QPushButton("Button 3");

    QLabel *label1 = new QLabel("Label 1");
    QLabel *label2 = new QLabel("Label 2");
    QLabel *label3 = new QLabel("Label 3");

    rightVbox->addWidget(button1);
    rightVbox->addWidget(button2);
    rightVbox->addWidget(button3);
    leftVbox->addWidget(label1);
    leftVbox->addWidget(label2);
    leftVbox->addWidget(label3);

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

} // namespace
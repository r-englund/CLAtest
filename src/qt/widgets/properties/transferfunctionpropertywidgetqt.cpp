#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

namespace inviwo {

TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property){
    generateWidget();
    updateFromProperty();
}


TransferFunctionPropertyWidgetQt::~TransferFunctionPropertyWidgetQt(){
	delete btnOpenDialog_;
	delete transferFunctionDialog_;
}


void TransferFunctionPropertyWidgetQt::generateWidget(){
    QHBoxLayout* hLayout = new QHBoxLayout();
	

	InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
	transferFunctionDialog_ = new TransferFunctionPropertyDialog(property_, app->getMainWindow());
	transferFunctionDialog_->setVisible(false);
	app->getMainWindow()->addDockWidget(Qt::BottomDockWidgetArea, transferFunctionDialog_);
	

    btnOpenDialog_ = new QPushButton();
    btnOpenDialog_->setFixedWidth(300);
    btnOpenDialog_->setFixedHeight(30);
	btnOpenDialog_->setText("Open TransferFunctionEditor");
    
	connect(btnOpenDialog_,SIGNAL(clicked()),this,SLOT(openTransferFunctionDialog()));
    hLayout->addWidget(btnOpenDialog_);
    setLayout(hLayout);
}

void TransferFunctionPropertyWidgetQt::updateFromProperty(){}

void TransferFunctionPropertyWidgetQt::setPropertyValue(){}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
	//transferFunctionDialog_->show();

	transferFunctionDialog_->setVisible(true);
}

}//namespace
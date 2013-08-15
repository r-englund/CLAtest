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
    btnOpenTF_ = new QToolButton();
    btnOpenTF_->setIcon(QIcon(QPixmap::grabWidget(transferFunctionDialog_->getGradient())));
    
    
    btnOpenTF_->setIconSize(QSize(300, 50));
    btnOpenTF_->setFixedSize(300,50);

	connect(btnOpenTF_,SIGNAL(clicked()),this,SLOT(openTransferFunctionDialog()));
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(btnOpenTF_);
    setLayout(hLayout);
}

void TransferFunctionPropertyWidgetQt::updateFromProperty(){
    btnOpenTF_->setIcon(QIcon(QPixmap::grabWidget(transferFunctionDialog_->getGradient())));
}


void TransferFunctionPropertyWidgetQt::setPropertyValue(){}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
	transferFunctionDialog_->setVisible(true);
}



}//namespace
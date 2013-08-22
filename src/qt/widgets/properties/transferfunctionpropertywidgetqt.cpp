#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

namespace inviwo {

TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property){
    generateWidget();
    updateFromProperty();
}


TransferFunctionPropertyWidgetQt::~TransferFunctionPropertyWidgetQt(){
    delete gradientView_;
    delete gradientScene_;
    delete gradient_;
    delete btnOpenTF_;
	delete transferFunctionDialog_;
}


void TransferFunctionPropertyWidgetQt::generateWidget(){
    QHBoxLayout* hLayout = new QHBoxLayout();
	
	InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
	transferFunctionDialog_ = new TransferFunctionPropertyDialog(property_, app->getMainWindow());
	transferFunctionDialog_->setVisible(false);
	app->getMainWindow()->addDockWidget(Qt::BottomDockWidgetArea, transferFunctionDialog_);
	
    gradientView_ = new QGraphicsView();
    gradientScene_ = new QGraphicsScene();
    gradientView_->setScene(gradientScene_);
    gradientView_->setFixedSize(150,25);
    gradientView_->setAlignment(Qt::AlignLeft);

    gradient_ = new QLinearGradient(0,0,gradientView_->width(),0);
    gradient_->setStops(*transferFunctionDialog_->getGradientStops());
    gradientView_->setForegroundBrush(*gradient_);
    gradient_->setFinalStop(gradientView_->width(),0);

    btnOpenTF_ = new QToolButton();
    btnOpenTF_->setIcon(QIcon(QPixmap::grabWidget(gradientView_)));
    btnOpenTF_->setIconSize(QSize(150,25));

	connect(btnOpenTF_,SIGNAL(clicked()),this,SLOT(openTransferFunctionDialog()));
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(btnOpenTF_);

    setLayout(hLayout);
}

void TransferFunctionPropertyWidgetQt::updateFromProperty(){
    gradient_->setFinalStop(gradientView_->width(), 0.0);
    gradient_->setStops(*transferFunctionDialog_->getGradientStops());
    gradientView_->setForegroundBrush(*gradient_);
    btnOpenTF_->setIcon(QIcon(QPixmap::grabWidget(gradientView_)));
}


void TransferFunctionPropertyWidgetQt::setPropertyValue(){}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
	transferFunctionDialog_->setVisible(true);
}



}//namespace
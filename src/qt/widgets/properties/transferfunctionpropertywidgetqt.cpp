#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

namespace inviwo {

TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property){
    PropertyWidgetQt::setProperty(property_);
    generateWidget();
    updateFromProperty();
    PropertyWidgetQt::generateContextMenu();
}


TransferFunctionPropertyWidgetQt::~TransferFunctionPropertyWidgetQt(){
    delete transferFunctionDialog_;
    delete gradient_;
    delete gradientView_;
    delete gradientScene_;
    delete btnOpenTF_;
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
    QPixmap pixmap(gradientView_->size());
    QPainter painter(&pixmap);
    gradientView_->render(&painter);
    btnOpenTF_->setIcon(QIcon(pixmap));
    btnOpenTF_->setIconSize(QSize(150,25));

    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        btnOpenTF_->setDisabled(true);
    }
    else{
        label_ = new EditableLabelQt(property_->getDisplayName());
        hLayout->addWidget(label_);
        connect(btnOpenTF_,SIGNAL(clicked()),this,SLOT(openTransferFunctionDialog()));
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    }

    hLayout->addWidget(btnOpenTF_);

    setLayout(hLayout);
}

void TransferFunctionPropertyWidgetQt::updateFromProperty(){
    if (gradientView_) {
        QVector<QGradientStop> stops = *transferFunctionDialog_->getGradientStops();
        gradient_->setFinalStop(gradientView_->width(), 0.0);
        gradient_->setStops(*transferFunctionDialog_->getGradientStops());
        gradientView_->setForegroundBrush(*gradient_);
        QPixmap pixmap(gradientView_->size());
        QPainter painter(&pixmap);
        gradientView_->render(&painter);
        btnOpenTF_->setIcon(QIcon(pixmap));
    }
}


void TransferFunctionPropertyWidgetQt::setPropertyValue(){}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
    transferFunctionDialog_->setVisible(true);
}

void TransferFunctionPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}




}//namespace
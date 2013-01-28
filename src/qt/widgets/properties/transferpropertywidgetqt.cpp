#include "inviwo/qt/widgets/properties/transferpropertywidgetqt.h"

namespace inviwo {

	TransferPropertyWidgetQt::TransferPropertyWidgetQt(TransferProperty* property) : property_(property) {
		generateWidget();
		updateFromProperty();
	}

	void TransferPropertyWidgetQt::generateWidget() {
		QVBoxLayout* hLayout = new QVBoxLayout();
		hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
		checkBox_ = new QCheckBox();

		int w = 160;
		int h = 40;

		QLabel* labelRed = new QLabel(QObject::tr("labelRed"));
		QLabel* labelGreen = new QLabel(QObject::tr("labelGreen"));
		QLabel* labelBlue = new QLabel(QObject::tr("labelBlue"));
		QLabel* labelWhite = new QLabel(QObject::tr("labelWhite"));

		QPixmap pixmapRed(w,h);
		QPixmap pixmapGreen(w,h);
		QPixmap pixmapBlue(w,h);
		QPixmap pixmapWhite(w,h);

		QLinearGradient grad(0, 0, 0, h);

		grad.setColorAt(0.0, Qt::red);
		grad.setColorAt(1.0, Qt::black);
		QPainter painterRed(&pixmapRed);
		painterRed.fillRect(0, 0, w, h, grad);
		labelRed->setPixmap(pixmapRed);

		grad.setColorAt(0.0, Qt::green);
		grad.setColorAt(1.0, Qt::black);
		QPainter painterGreen(&pixmapGreen);
		painterGreen.fillRect(0, 0, w, h, grad);
		labelGreen->setPixmap(pixmapGreen);

		grad.setColorAt(0.0, Qt::blue);
		grad.setColorAt(1.0, Qt::black);
		QPainter painterBlue(&pixmapBlue);
		painterBlue.fillRect(0, 0, w, h, grad);
		labelBlue->setPixmap(pixmapBlue);

		grad.setColorAt(0.0, Qt::white);
		grad.setColorAt(1.0, Qt::black);
		QPainter painterWhite (&pixmapWhite);
		painterWhite.fillRect(0, 0, w, h, grad);
		
		QPen pen(Qt::white, 2, Qt::SolidLine);
		pen.setColor(QColor(255, 255, 255, 127));
		painterWhite.setPen(pen);
		painterWhite.drawLine(w, h, w/2, h/2);
		painterWhite.drawLine(0, h, w/2, h/2);
		painterWhite.drawEllipse(w/2 - 5, h/2 - 5, 10, 10);
		labelWhite->setPixmap(pixmapWhite);

		connect(checkBox_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));

		hLayout->addWidget(labelRed);
		hLayout->addWidget(labelGreen);
		hLayout->addWidget(labelBlue);
		hLayout->addWidget(labelWhite);
		setLayout(hLayout);
	}

	void TransferPropertyWidgetQt::setPropertyValue() {
		property_->set(checkBox_->isChecked());
	}

	void TransferPropertyWidgetQt::updateFromProperty() {
		checkBox_->setChecked(property_->get());
	}

} // namespace

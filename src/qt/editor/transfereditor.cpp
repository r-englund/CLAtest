
#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/core/metadata/positionmetadata.h>
#include <inviwo/core/util/inviwofactorybase.h>

#include <inviwo/qt/editor/transfereditor.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QVarLengthArray>

namespace inviwo {

	const std::string TransferEditor::logSource_ = "TransferEditor";

	void TransferEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
		LogInfo("Transfereditor click");
	}
} // namespace
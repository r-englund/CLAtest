#ifndef IVW_TRANSFEREDITOR_H
#define IVW_TRANSFEREDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>

#include "processorgraphicsitem.h"
#include "connectiongraphicsitem.h"
#include "linkgraphicsitem.h"
#include "linkdialog.h"

namespace inviwo {

	class CanvasQt;

	class IVW_QTEDITOR_API TransferEditor : public QGraphicsScene {

	public:
		static TransferEditor* instance();
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* e);
		
	private:
		static const std::string logSource_; ///< Source string to be displayed for log messages
	};

} // namespace

#endif // IVW_TRANSFEREDITOR_H
#ifndef IVW_SPINBOXRANGESLIDERQT_H
#define IVW_SPINBOXRANGESLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/customsliderwidgetqt.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <QHBoxLayout>
#include <QSpinBox>

namespace inviwo {

	class IVW_QTWIDGETS_API SpinBoxRangeSliderQt : public QWidget {
		Q_OBJECT;
	public:
		SpinBoxRangeSliderQt();
		virtual ~SpinBoxRangeSliderQt();
		const int getMinValue();
		const int getMaxValue();
		const int getMinRange();
		const int getMaxRange();
		void setMinValue(const int value);
		void setMaxValue(const int value);
		void setMinRange(const int minRange);
		void setMaxRange(const int maxRange);
		void setMinMaxRange(const int minRange, const int maxRange);
		
	private slots:
		void updateFromSpinBoxMin();
		void updateFromSpinBoxMax();
		void updateFromSlider(int valMin, int valMax);

	signals:
		void valuesChanged();

	private:
		RangeSliderQt* slider_;
		QSpinBox* spinBoxMin_;
		QSpinBox* spinBoxMax_;
		int minRange_;
		int maxRange_;
	};

}//namespace

#endif // IVW_SPINBOXRANGESLIDERQT_H
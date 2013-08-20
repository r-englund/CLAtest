/** \
*
*/

#ifndef IVW_TRANSFERFUNCTIONDATAPOINT_H
#define IVW_TRANSFERFUNCTIONDATAPOINT_H
#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/ports/imageport.h>
//#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>

namespace inviwo {

    class IVW_CORE_API TransferFunctionDataPoint {

    public:
		TransferFunctionDataPoint();
		TransferFunctionDataPoint(vec2* pos);
		TransferFunctionDataPoint(vec2 pos, vec4 rgba);
		TransferFunctionDataPoint(vec2* pos, vec4* rgba);

        virtual ~TransferFunctionDataPoint();

		const vec2* getPos();
		void setPos(vec2 pos);
		void setPos(vec2* pos);
		void setPos(float x, float y);
		const vec4* getRgba();
		void setRgba(vec4 rgba);
		void setRgba(vec4* rgba);
        void setRgb(const vec3* rgb);
        void setA(const float alpha);
		const bool isSelected();
		void setSelected(bool);

    private:
		vec2 pos_;
        vec4 rgba_;
		bool selected_;
    };

} // namespace
#endif // IVW_TRANSFERFUNCTIONDATAPOINT_H

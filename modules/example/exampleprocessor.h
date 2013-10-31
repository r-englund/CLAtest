#ifndef IVW_EXAMPLEPROCESSOR_H
#define IVW_EXAMPLEPROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

	class IVW_MODULE_BASE_API ExampleProcessor : public ProcessorGL {
	public:
		ExampleProcessor();
		~ExampleProcessor();

		InviwoProcessorInfo();

		void initialize();
		void deinitialize();

	protected:
		virtual void process();

	private:
		VolumeInport inport_;
		GeometryOutport outport_;

		BoolProperty clippingEnabled_;

		IntMinMaxProperty clipX_;
		IntMinMaxProperty clipY_;
		IntMinMaxProperty clipZ_;

		glm::uvec3 dims_;
		glm::mat4 basis_;
	};

} // namespace

#endif // IVW_EXAMPLEPROCESSOR_H

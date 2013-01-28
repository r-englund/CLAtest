#include "transferfunction.h"
#include "stdio.h"

namespace inviwo {

	TransferFunction::TransferFunction()
		: ProcessorGL(),
		//bool_("bool", "boolthing", false),
		trans_("trans", "transfer", true)
		//bool2_("bool2", "boolthing2", false)
	{
		//addProperty(bool_);
		addProperty(trans_);
		//addProperty(bool2_);
	}

	TransferFunction::~TransferFunction() {}

	Processor* TransferFunction::create() const {
		return new TransferFunction();
	}

	void TransferFunction::initialize() {
		ProcessorGL::initialize();
		shader_ = new Shader("img_findedges.frag");
	}

	void TransferFunction::deinitialize() {
		delete shader_;
		Processor::deinitialize();
	}

	void TransferFunction::process() {
		deactivateCurrentTarget();
	}

} // namespace

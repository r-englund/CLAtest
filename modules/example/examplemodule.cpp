#include <modules/example/examplemodule.h>

#include <modules/example/exampleprocessor.h>

namespace inviwo {

ExampleModule::ExampleModule() : InviwoModule() {
    setIdentifier("Example");
    setXMLFileName("example/examplemodule.xml");

    registerProcessor(ExampleProcessor);
	
}

} // namespace
             
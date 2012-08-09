#include "inviwo/core/util/project.h"

namespace inviwo {

Project::Project() {}
Project::~Project() {}

void Project::load(std::string projectName) throw (Exception) {
    std::cout << "loading project " << projectName << "..." << std::endl;

    std::fstream fileStream(projectName.c_str(), std::ios_base::in);
    if (fileStream.fail())
        throw SerializationException("Could not open project '" + projectName + "'.");

    XmlDeserializer d(projectName);
    d.setUseAttributes(true);
    NetworkSerializer ser;
    try {
        d.read(fileStream, &ser);
    }
    catch (SerializationException& e) {
        throw SerializationException("Failed to read serialization data stream from workspace file '"
            + projectName + "': " + e.what());
    }
    catch (...) {
        throw SerializationException("Failed to read serialization data stream from workspace file '"
            + projectName + "' (unknown exception).");
    }

    // deserialize workspace from data stream
    try {
        d.deserialize("Workspace", *this);
        //errorList_ = d.getErrors();
        //setFileOrigin(projectName);
    }
    catch (std::exception& e) {
        throw SerializationException("Deserialization from workspace file '" + projectName + "' failed: " + e.what());
    }
    catch (...) {
        throw SerializationException("Deserialization from workspace file '" + projectName + "' failed (unknown exception).");
    }
}

ProcessorNetwork* Project::getProcessorNetwork() {
    return processorNetwork_;
}

void Project::serialize(XmlSerializer& /*s*/) const {}
void Project::deserialize(XmlDeserializer& s) {
    std::cout << "deserializing project" << std::endl;
    // Deserialize network...
    s.deserialize("ProcessorNetwork", processorNetwork_);
}

} // namespace

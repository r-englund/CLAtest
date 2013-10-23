#include <inviwo/core/io/datawriter.h>

namespace inviwo {

DataWriter::DataWriter()
    : identifier_("undefined")
{}

DataWriter::~DataWriter(){

}

std::string DataWriter::getIdentifier() const{
    return identifier_;
}

void DataWriter::setIdentifier(const std::string& name){
    identifier_ = name;
}

} // namespace

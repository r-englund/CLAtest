#include <inviwo/core/io/datareader.h>

namespace inviwo {

    DataReader::DataReader()
        : identifier_("undefined")
    {}

    DataReader::DataReader(std::string identifier)
        : identifier_(identifier)
    {}

} // namespace

#include <inviwo/core/util/fileextension.h>

namespace inviwo {

FileExtension::FileExtension()
    : extension_("txt")
    , description_("Textfile") {};
FileExtension::FileExtension(std::string extension, std::string description)
    : extension_(extension)
    , description_(description) {};

} // namespace


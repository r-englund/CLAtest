#include <inviwo/core/util/dialogfactory.h>

namespace inviwo {

DialogFactory::DialogFactory() {}

DialogFactory::~DialogFactory() {}

void DialogFactory::registerObject(DialogFactoryObject* dialog) {
    std::string className = dialog->getClassIdentifier();

    if (map_.find(className) != map_.end()) {
        LogWarn("Dialog already registed: " << className);
    }

    map_.insert(std::make_pair(className, dialog));
}


Dialog* DialogFactory::getDialog(std::string className) const {
    DialogMap::const_iterator it = map_.find(className);

    if (it != map_.end()) {
        return it->second->create();
    } else {
        return NULL;
    }
}

IvwSerializable* DialogFactory::create(std::string className) const {
    // Dialogs are not serializable...
    return 0;
}

bool DialogFactory::isValidType(std::string className) const {
    if (map_.find(className) != map_.end())
        return true;
    else
        return false;
}

} // namespace


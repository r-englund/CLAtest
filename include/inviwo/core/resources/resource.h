#ifndef IVW_RESOURCE_H
#define IVW_RESOURCE_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

/** \class Resource 
* This is an abstract class for Resources.
* A Resource is a container for data. The data is owned by the Resource and deleted upon Resource destruction.
* Resource implementations must implement the == operator for both other resources and a string identifier. 
* The string identifier is used to request resources from the resource manager and should be unique.
* @see ResourceTemplate
* @see ResourceManager
*/
class IVW_CORE_API Resource {

public:
    Resource() {};

    /**
     * Child classes are responsible for deleting the resource.
     * 
     * @return 
     */
    virtual ~Resource() {};

    ///**
    // * Compare two resources.
    // * 
    // * @param other Other resource object.
    // * @return True if equal, false otherwise.
    // */
    //virtual bool operator==(const Resource& other) const = 0;
    //bool operator!=(const Resource& other) const {
    //    return !(*this == other);
    //}
    /**
     * Compare a resource using an identifier.
     * 
     * @param identifier Identifier for resource object.
     * @return True if equal, false otherwise.
     */
    bool operator==(const std::string& identifier) const { return identifier == getIdentifier(); };
    bool operator!=(const std::string& identifier) const {
        return !(*this == identifier);
    }

    /**
     * Get a unique identifier for the resource. For example file path can often uniquely define a resource.
     * 
     * @return Unique identifier for object.
     */
    virtual const std::string& getIdentifier() const = 0;
};

} // namespace

#endif // IVW_RESOURCE_H
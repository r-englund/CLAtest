/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_RESOURCE_MANAGER_H
#define IVW_RESOURCE_MANAGER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/resources/resource.h>
#include <inviwo/core/resources/resourcemanagerobserver.h>
#include <inviwo/core/util/singleton.h>
#include <vector>

namespace inviwo {
/** \class ResourceManager
* Manager for resources.
* Resources added are owned by the ResourceManager and they will be deleted when the ResourceManager is destroyed.
* Use an identifier to find resources added to the ResourceManager. The identifier can be the file name if it is a file resource.
* @see ResourceTemplate
*/
// TODO: Should we add resource counting?
// TODO: How do we generate identifiers for different resources?
class IVW_CORE_API ResourceManager: public ResourceManagerObservable {

public:
    static ResourceManager* instance() {
        static ResourceManager instance;// Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }
    ~ResourceManager();


    /**
     * Add resource to ResourceManager.
     * Will take ownership of the resource.
     *
     * @param resource Resource to add.
     */
    void addResource(Resource* resource) { resources_->push_back(resource); notifyResourceAdded(resource); }

    /**
     * Remove resource from ResourceManager.
     * This will delete the resource.
     *
     * @param resource Resource to remove.
     */
    void removeResource(Resource* resource);

    /**
     * Remove resource from ResourceManager using the identifier.
     * This will delete the resource.
     *
     * @param identifier Identifier of resource to remove.
     */
    void removeResource(const std::string& identifier);

    /**
     * Clear all resource in ResourceManager.
     */
    void clearAllResources();

    /**
     * Get Resource using identifier.
     *
     * @param identifier Resource identifier.
     * @return NULL if the resource was not found, otherwise pointer to resource.
     */
    Resource* getResource(const std::string& identifier);

    /**
     * Get Resource using identifier and cast it to type.
     *
     * @param identifier Resource identifier.
     * @return NULL if the resource was not found or dymanic cast failed, otherwise pointer to resource.
     */
    template<typename T> T* getResourceAs(const std::string& identifier);

    /**
     * Check if ResourceManager has resource.
     *
     * @param identifier Resource identifier.
     * @return True if it exists, false otherwise.
     */
    bool hasResource(const std::string& identifier) { return getResource(identifier) != NULL; };
    /**
     * Returns a vector of Resources which are of type T. In case no Resources match T
     * an empty vector is returned.
     *
     */
    template<typename T> std::vector<T*> getResourcesByType() const {
        std::vector<T*> typedResources;

        for (std::vector<Resource*>::const_iterator it = resources_->begin(); it != resources_->end(); ++it) {
            if (dynamic_cast<T*>(*it))
                typedResources.push_back(*it);
        }

        return typedResources;
    }

    std::vector<Resource*>* getResources() { return resources_; }

protected:

private:
    ResourceManager(): ResourceManagerObservable() {
        resources_ = new std::vector<Resource*>();
    };
    ResourceManager(ResourceManager const&) {
        resources_ = new std::vector<Resource*>();
    };
    void operator=(ResourceManager const&) {};

    std::vector<Resource*>* resources_;
};

template<typename T>
T* inviwo::ResourceManager::getResourceAs(const std::string& identifier)
{
    return dynamic_cast<T*>(getResource(identifier));
}

} // namespace

#endif // IVW_RESOURCE_MANAGER_H
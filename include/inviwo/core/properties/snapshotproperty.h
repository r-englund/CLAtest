#ifndef IVW_SNAPSHOTPROPERTY_H
#define IVW_SNAPSHOTPROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {
/** \brief Property for saving a snapshot of the specified ImagePort.
 *
 *  The internal value of the property is the file path to where the
 *  snapshot should be saved. The property is also linked to an ImagePort,
 *  which may be set using setImagePort.
 */
class IVW_CORE_API SnapshotProperty : public TemplateProperty<std::string> {

public:
    SnapshotProperty(std::string identifier, std::string displayName, ImagePort& imagePort, PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual ~SnapshotProperty();

    /**
     * Set the internal imagePort.
     * @param imagePort The new imagePort.
     */
    void setImagePort(ImagePort& imagePort) { imagePort_ = &imagePort; }

    /**
     * Save a snapshot of imagePort to file.
     * The file path used is stored in the value_ member.
     */
    void saveSnapshot();

private:
    /**
     * The imagePort from which the snapshot should be taken.
     */
    ImagePort* imagePort_;

};

} // namespace

#endif // IVW_SNAPSHOTPROPERTY_H
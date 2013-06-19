#ifndef IVW_INTERACTIONHANDLER_H
#define IVW_INTERACTIONHANDLER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/eventlistener.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

    class IVW_CORE_API InteractionHandler : public EventListener, public IvwSerializable {

    public:
        InteractionHandler();
        virtual ~InteractionHandler();

		virtual std::string getClassName() const { return "undefined"; }

		void serialize(IvwSerializer &s) const;
		void deserialize(IvwDeserializer &d);
    };

} // namespace

#endif // IVW_INTERACTIONHANDLER_H
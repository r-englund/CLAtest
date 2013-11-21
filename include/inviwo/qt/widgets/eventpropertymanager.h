#ifndef IVW_EVENTPROPERTYMANAGER_H
#define IVW_EVENTPROPERTYMANAGER_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/properties/eventproperty.h>
#include <vector>

namespace inviwo {

/** class EventPropertyManager
* 
* A manager which finds and contain all eventproperties in the workspace.
* All remapping of existing eventproperties are done through the manager.
* The eventpropertymanager is owned by the mappingwidget and the calls
* to the remap-functions comes from MappingPopup.
* @see EventProperty
* @see MappingWidget
* @see MappingPopup
*/
class IVW_QTWIDGETS_API EventPropertyManager : public VoidObservable {

public:
	EventPropertyManager();
	~EventPropertyManager();

	/** 
	 * \brief Returns eventproperties
	 *
	 * Returns the eventproperties for the current active processor.
	 * The active processor is the processor which is seletected in the combobox in the mappingwidget.
	 * 
	 * @return std::vector<EventProperty*> A vector with the eventproperties for the active processor.
	 */
	std::vector<EventProperty*> getEventPropertiesFromMap();

	/** 
	 * \brief Updates the manager with eventproperties
	 *
	 * Sets the map of eventproperties to a new map of all current eventproperties.
	 * This function is called every time a processor is added or removed from the workspace.
	 * 
	 * @param std::map<std::string, std::vector<EventProperty * > > eventPropertyMap The new map of eventpropeties
	 * @return void
	 */
	void setEventPropertyMap(std::map<std::string, std::vector<EventProperty*> > eventPropertyMap);
	
	/** 
	 * \brief Sets the active processor
	 *
	 * Sets the active processor to the currently selected processor in the combobox in the mappingwidget.
	 * The active processor is stored as a string of the processors identifier, which is also used as key in the eventPropertyMap.
	 * 
	 * @param std::string processorIdentifier The identifier of the selected processor
	 * @return void
	 */
	void setActiveProcessor(std::string processorIdentifier);

	/** 
	 * \brief Remaps an eventproperty with a mouseevent
	 *
	 * Changes the mapping of an eventproperty with a mouseevent and a modifier.
	 * If another eventproperty is mapped with the same event and modifier, that eventpropertys mapping is unbound.
	 * This function is called from the MappingPopup.
	 * 
	 * @param EventProperty * eventProperty The eventproperty to remap
	 * @param MouseEvent::MouseButton button The buton for the new mouseevent
	 * @param Event::Modifier modifier The modifier for the new mouseevent
	 * @return void
	 */
	void changeMouseMapping(EventProperty* eventProperty, MouseEvent::MouseButton button, InteractionEvent::Modifier modifier);
	
	/** 
	 * \brief Remaps an eventproperty with a keyboardevent
	 *
	 * Changes the mapping of an eventproperty with a keyboardevent and a modifier.
	 * If another eventproperty is mapped with the same event and modifier, that eventpropertys mapping is unbound.
	 * This function is called from the MappingPopup.
	 * 
	 * @param EventProperty * eventProperty The eventproperty to remap
	 * @param char button The button for the new keyboardevent
	 * @param Event::Modifier modifier The modifier for the new keyboardevent
	 * @return void
	 */
	void changeKeyMapping(EventProperty* eventProperty, char button, InteractionEvent::Modifier modifier);

	bool isEmpty();

private:
	std::map<std::string, std::vector<EventProperty*> > eventPropertyMap_; ///< map<Processor identifier, vector of the processors eventproperties>
	std::string activeProcessor_; ///< The identifier of the currently seletected processor in the combobox in the mappingwidget
};

} // namespace

#endif // IVW_EVENTPROPERTYMANAGER_H
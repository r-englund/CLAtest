#ifndef IVW_OBSERVER_H
#define IVW_OBSERVER_H

#include <inviwo/core/inviwocoredefine.h>
#include <set>

namespace inviwo {
// Forward declaration
class ObservableInterface;

/** \class Observer 
*
* Class to support observer pattern. An example of usage is given in the Observable class.
* @see Observable
*/
class Observer
{
friend class ObservableInterface;
public:
	/** 
	 * Allocates Observable set.
	 */
	Observer();

	/**
	 * Removes the observer from all observable objects. Makes sure that it cannot be called when destroyed.
	 * 
	 */
    virtual ~Observer();

	/**
	 * Stop observing object by removing it from observation list.
	 * 
	 * \param observable (ObservableInterface *) The observable to stop observing.
	 */
    void removeObservation(ObservableInterface* observable);

	/**
	 * Stops observing all objects by removing them from observation list.
	 * 
	 */
    void removeObservations();

protected:
	/** 
	 * Add an object to observe.
	 * 
	 * @param observable The observable to add.
	 */
	void addObservation(ObservableInterface* observable);


    // Storing observables connected to this observer enables removal upon destruction.
	typedef std::set<ObservableInterface*> ObservableSet;
	ObservableSet* observables_; 
};

/** \class ObservableInterface 
*
* Class to support observer pattern. 
* This is an interface only, inherit from Observable<DerivedObserver> to define your own "notify()" method. 
* An example of usage is given in the Observable class.
* @see Observable
* @see Observer
*/
class ObservableInterface
{
friend class Observer;
public:
    /** 
	 * Allocates Observer set.
	 */
	ObservableInterface();
	/* 
	 * Removes all observers.
	 */
	virtual ~ObservableInterface();

protected:
	/** 
	 * Add an observer.
	 * 
	 * @param observer The observer to add.
	 */
	virtual void addObserver(Observer* observer);
 	/** 
	 * Remove an observer.
	 * 
	 * @param observer The observer to remove.
	 */
	virtual void removeObserver(Observer* observer);
	/** 
	 * Remove all observers.
	 */
	void removeObservers();

	typedef std::set<Observer*> ObserverSet;
	ObserverSet* observers_; 

};

/** \class Observable 
*
* Class to support observer pattern. 
*
* \section example Example
* Example of how to apply it to a simple button.
* @code
*    class IVW_XXX_API ButtonObserver: public Observer {
*    public:
*        ButtonObserver(): Observer() {};
*        // Will be notified when the observed button is pressed.
*        void buttonPressed(){};
*    };
*
*    class IVW_XXX_API Button: public Observable<ButtonObserver> {
*        Button(): Observable<ButtonObserver>() {};
*        void pressButton() {
*            // Do stuff
*            // Notify observers
*            ObserverSet::iterator endIt = observers_->end();
*            for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
*               // static_cast can be used since only template class objects can be added
*               static_cast<ButtonObserver*>(*it)->buttonPressed();    
*            }
*        }
*    };
* @endcode
* @see Observer
*/
template<typename T>
class IVW_CORE_API Observable: public ObservableInterface {
public:
    Observable(): ObservableInterface() {};
    /**
     * Removes all observers 
     */
    virtual ~Observable() {};
	/** 
	 * Add an observer.
	 * 
	 * @param observer The observer to add.
	 */
    virtual void addObserver(T* observer) { 
        ObservableInterface::addObserver(observer); 
    }
 	/** 
	 * Remove an observer.
	 * 
	 * @param observer The observer to remove.
	 */
    virtual void removeObserver(T* observer) {
        ObservableInterface::removeObserver(observer);
    }
};

} // namespace

#endif // IVW_OBSERVER_H

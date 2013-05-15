#ifndef IVW_OBSERVER_H
#define IVW_OBSERVER_H

#include <inviwo/core/common/inviwocoredefine.h>
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
*    public:
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
* @see VoidObserver
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


/** \class VoidObserver 
*
* Simple observer that does not require any arguments
* when observed object changes. The observer therefore 
* does not know what changed, only that it changed.
*
* @see VoidObservable
*/
class IVW_CORE_API VoidObserver: public Observer {
public:
    VoidObserver(): Observer() {};

    /**
    * This method will be called when observed object changes.
    * Override it to add behavior.
    */
    virtual void notify() {};
};


/** \class VoidObservable 
*
* Simple observable that should call notifyObservers as soon as it 
* has changed. The observer  
* does not know what changed, only that it changed.
*
* @see VoidObserver
*/
class IVW_CORE_API VoidObservable: public Observable<VoidObserver> {
public:
    VoidObservable(): Observable<VoidObserver>() {};


    void notifyObservers() const {
        // Notify observers
        ObserverSet::iterator endIt = observers_->end();
        for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<VoidObserver*>(*it)->notify();    
        }
    }
};

} // namespace

#endif // IVW_OBSERVER_H

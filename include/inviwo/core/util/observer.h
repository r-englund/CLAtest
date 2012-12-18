#ifndef IVW_OBSERVER_H
#define IVW_OBSERVER_H

#include <set>
#include <vector>
#include <algorithm>

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
	/* 
	 * Allocates Observable set.
	 */
	Observer();
	/* 
	 * Removes the observer from all observable objects. Makes sure that it cannot be called when destroyed.
	 */
	virtual ~Observer();

 	/* 
	 * Stop observing object by removing it from observation list.
	 * 
	 * @param observable The observable to stop observing.
	 */
	void removeObservation(ObservableInterface* observable);

	/* 
	 * Stops observing all objects by removing them from observation list.
	 */
	void removeObservations();

protected:
	/* 
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
    /* 
	 * Allocates Observer set.
	 */
	ObservableInterface();
	/* 
	 * Removes all observers.
	 */
	virtual ~ObservableInterface();

protected:
	/* 
	 * Add an observer.
	 * 
	 * @param observer The observer to add.
	 */
	virtual void addObserver(Observer* observer);
 	/* 
	 * Remove an observer.
	 * 
	 * @param observer The observer to remove.
	 */
	virtual void removeObserver(Observer* observer);
	/* 
	 * Remove all observers.
	 */
	void removeObservers();

	typedef std::set<Observer*> ObserverSet;
	ObserverSet* observers_; 

};

/** \class Observable 
*
* Class to support observer pattern. 
* Template class must inherit from the Observer class. 
* @code
*    class ButtonObserver: public Observer {
*    public:
*        ButtonObserver(): Observer() {};
*        // Will be notified when the observed button is pressed.
*        void buttonPressed(){};
*    };
*    class Button: public Observable<ButtonObserver> {
*        Button(): Observable<ButtonObserver>() {};
*        void pressButton() {
*            // Do stuff
*            // Notify observers
*            std::vector<ButtonObserver*> observers = getObservers();
*            for(std::vector<ButtonObserver*>::iterator it = observers.begin(); it != observers.end(); ++it) {
*                (*it)->buttonPressed();    
*            }
*        }
*    };
* @endcode
* @see Observer
*/
template<typename T>
class Observable: public ObservableInterface {
public:
    Observable(): ObservableInterface() {};
    /*
     * Removes all observers 
     */
    virtual ~Observable() {};
	/* 
	 * Add an observer.
	 * 
	 * @param observer The observer to add.
	 */
    virtual void addObserver(T* observer) { 
        ObservableInterface::addObserver(observer); 
    }
 	/* 
	 * Remove an observer.
	 * 
	 * @param observer The observer to remove.
	 */
    virtual void removeObserver(T* observer) {
        ObservableInterface::removeObserver(observer);
    }

protected:
    // Helper to copy observers
    template <typename From, typename To>
    struct static_caster
    {
        To operator()(From p) {return static_cast<To>(p);}
    };
 	/* 
	 * Get all observers. Convenience function that type casts all observers to type T.
	 * 
	 * @return All observers observing the object.
	 */
    std::vector<T*> getObservers() { 
        std::vector<T*> observers(observers_->size());
        std::transform(observers_->begin(), observers_->end(), observers.begin(), static_caster<Observer*, T*>());
        return observers; 
    }

};



} // End of namespace




#endif // IVW_OBSERVER_H

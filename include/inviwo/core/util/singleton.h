#ifndef IVW_SINGLETON_H
#define IVW_SINGLETON_H

#include <inviwo/core/util/assertion.h>
#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

template <class T>
class Singleton {

public:
    static void init() {
        ivwAssert(instance_==0, "Singleton already initialized.");
        instance_ = new T;
    };

    static void init(T* instance) {
        ivwAssert(instance_==0, "Singleton already initialized.");
        ivwAssert(instance!=0, "Null pointer passed.");
        instance_ = instance;
    };

    static T& getRef() {
        ivwAssert(instance_!=0, "Singleton not initialized. Ensure that init() "
                                "is called in a thread-safe environment.");
        return *instance_;
    };

    static T* getPtr() {
        ivwAssert(instance_!=0, "Singleton not initialized. Ensure that init() "
                                "is called in a thread-safe environment.");
        return instance_;
    };

protected:
    Singleton<T>() {};
    ~Singleton() {};
    
private:    
    // no implementation for copy functionalities by making the following two methods
    // unaccessible. Otherwise unwanted Singleton copies might appear.
    Singleton<T>(Singleton<T> const&);
    void operator=(Singleton<T> const&);

    static T* instance_;
};

template <class T> T* Singleton<T>::instance_ = 0;

} // end of namespace

#endif // IVW_SINGLETON_H

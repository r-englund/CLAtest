#ifndef IVW_DATAGROUP_H
#define IVW_DATAGROUP_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/datagrouprepresentation.h>

namespace inviwo {
/** \brief The base class for all DataGroup objects.
 *
 *  DataGroup is the base class for all the DataGroup objects.
 *  It owns or has reference to zero or many Data objects.
 *
 *  It also owns DataGroupRepresentations, which has references to DataRepresentations,
 *  which are owned by the referenced/owned Data objects.
 *
 *  Differences between DataGroup and Data:
 *    - DataGroup can never hold any data with owning/referencing a Data object
 *    - DataGroupRepresentation need reference to all Data objects to be created correctly
 *    - DataGroup does not have converters, as the DataGroup objects always can create them self correctly.
 *    - DataGroupRepresentation becomes invalid when a child representations
 */

class IVW_CORE_API DataGroup {

public:
    DataGroup();
    DataGroup(const DataGroup& rhs);
    DataGroup& operator=(const DataGroup& rhs);
    virtual DataGroup* clone() const = 0;
    virtual ~DataGroup();

    //Representations
    template<typename T>
    const T* getRepresentation() const;
    template<typename T>
    T* getEditableRepresentation();

    template<typename T>
    bool hasRepresentation() const;
    bool hasRepresentations() const;

    void addData(Data*);
    void clearRepresentations();

protected:
    std::vector<Data*> data_;
    mutable std::vector<DataGroupRepresentation*> representations_;
};

template<typename T>
const T* DataGroup::getRepresentation() const {
    // check if a representation exists and return it
    for (int i=0; i<static_cast<int>(representations_.size()); ++i) {
        T* representation = dynamic_cast<T*>(representations_[i]);
        if (representation) {
            DataGroupRepresentation* basRep = dynamic_cast<DataGroupRepresentation*>(representation);
            if(basRep){
                if (basRep->isValid()) {
                    return representation;
                } else {
                    basRep->update();
                    return representation;
                }
            }
        }
    }

    //no representation exists, create one
    T* result = new T(&data_);
    representations_.push_back(result);
    return result;    
}

template<typename T>
T* DataGroup::getEditableRepresentation() {
    bool hasRep = hasRepresentation<T>();
    T* result = const_cast<T*>(getRepresentation<T>());
    return result;
}

template<typename T>
bool DataGroup::hasRepresentation() const {
    for (int i=0; i<static_cast<int>(representations_.size()); i++) {
        T* representation = dynamic_cast<T*>(representations_[i]);
        if (representation) return true;
    }
    return false;
}

} // namespace

#endif // IVW_DATAGROUP_H

#ifndef IVW_DATAGROUPREPRESENTATION_H
#define IVW_DATAGROUPREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

/** \brief The base class for all DataGroupRepresentation objects.
 *
 *  It has reference to zero or many DataRepresentation objects, but never owns them,
 *  they are always owned by the Data object.
 *
 *  Differences between DataGroupRepresentation and DataRepresentation:
 *    - DataGroupRepresentation does not own DataRepresentation, does should never delete them.
 *    - DataGroupRepresentation becomes invalid when a child DataRepresentation is invalid.
 */

class IVW_CORE_API DataGroupRepresentation : DataRepresentation {

public:
    DataGroupRepresentation(std::vector<Data*>*);
    DataGroupRepresentation(const DataGroupRepresentation& rhs);
    virtual ~DataGroupRepresentation();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;
    virtual DataGroupRepresentation* clone() const = 0;
    virtual std::string getClassName() const;

    //Update representations_ with DataRepresentation from each Data object
    virtual void update() = 0;

    //Function for checking if representations_ is identical (and valid) to those in the Data object.
    virtual bool isValid() const = 0;

protected:
    std::vector<Data*>* data_;
    std::vector<DataRepresentation*> representations_;
};

} // namespace

#endif // IVW_DATAGROUPREPRESENTATION_H

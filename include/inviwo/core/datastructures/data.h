#ifndef IVW_DATA_H
#define IVW_DATA_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/datarepresentation.h"

namespace inviwo {

    class Data {

    public:
        Data();
        virtual ~Data();

        template<class T>
        bool hasRepresentation() const;
        template<class T>
        T* getRepresentation();
        void clearRepresentations();

    protected:
        std::vector<DataRepresentation*> representations_;

    };

    template<class T>
    T* Data::getRepresentation() {
        if (representations_.size() > 0) {
            // check if a representation exists and return it
            for (size_t i=0; i<representations_.size(); i++) {
                T* representation = dynamic_cast<T*>(representations_[i]);
                if (representation)
                    return representation;
            }
            // no representation exists, so we try to create one
            T* result = 0;
            // TODO: convert existing representations
            //converter = app.getRepresentationFactory().getConverter(T*); // how to deal with source
            // result = converter.convert(this);
            return result;
        } else {
            // no representation exists, so create one
            T* result = new T();
            representations_.push_back(result);
            return result;
        }
    }

    template<class T>
    bool Data::hasRepresentation() const {
        for (size_t i=0; i<representations_.size(); i++) {
            T* representation = dynamic_cast<T*>(representations_[i]);
            if (representation) return true;
        }
        return false;
    }

} // namespace

#endif // IVW_DATA_H

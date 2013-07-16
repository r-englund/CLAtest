#ifndef IVW_PYVARIANT_H
#define IVW_PYVARIANT_H

#include <modules/python/pythonmoduledefine.h>

#include <inviwo/core/util/variant.h>

namespace inviwo {

    class PyVariant : public Variant{
    public:
        explicit PyVariant(PyObject *obj, VariantType type);
        explicit PyVariant(PyObject *obj, int type);

        PyVariant(const Variant& obj);
        PyVariant(const VariantType& type);
        PyVariant(const int& type);

        void setPython(PyObject *obj, VariantType type);

        PyObject *getAsPythonObject();

    };

} //namespace

#endif // IVW_PYVARIANT_H
#ifndef IVW_VARIANT_H
#define IVW_VARIANT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/exception.h>

#include <vector>
#include <string>

namespace inviwo {

class IVW_CORE_API Variant : public IvwSerializable {
public:
    enum VariantType {
        // Don't change order
        VariantTypeInvalid = 0,
        VariantTypeBool = 1,
        VariantTypeDouble = 2,
        VariantTypeFloat = 3,
        VariantTypeInteger = 4,
        VariantTypeLong = 5,
        VariantTypeString = 6,
        VariantTypeIVec2 = 7,
        VariantTypeIVec3 = 8,
        VariantTypeIVec4 = 9,
        VariantTypeVec2 = 10,
        VariantTypeVec3 = 11,
        VariantTypeVec4 = 12,
        VariantTypeDVec2 = 13,
        VariantTypeDVec3 = 14,
        VariantTypeDVec4 = 15,
        VariantTypeMat2 = 16,
        VariantTypeMat3 = 17,
        VariantTypeMat4 = 18,
        VariantTypeLastBaseType = 19,
        VariantTypeUserType = 32
    };    
  
    Variant();
    Variant(const Variant& obj);
    explicit Variant(bool value);
    explicit Variant(double value);
    explicit Variant(float value);
    explicit Variant(int value);
    explicit Variant(long value);
    explicit Variant(const char* value);
    explicit Variant(const std::string& value);
    explicit Variant(const ivec2& value);
    explicit Variant(const ivec3& value);
    explicit Variant(const ivec4& value);
    explicit Variant(const vec2& value);
    explicit Variant(const vec3& value);
    explicit Variant(const vec4& value);
    explicit Variant(const dvec2& value);
    explicit Variant(const dvec3& value);
    explicit Variant(const dvec4& value);
    explicit Variant(const mat2& value) ;
    explicit Variant(const mat3& value) ;
    explicit Variant(const mat4& value) ;    
    explicit Variant(const VariantType &type);

    Variant deepCopy() const;
    ~Variant();
    VariantType getType() const;

    bool isValid() const;
    static std::string typeToName(VariantType type);
    static VariantType nameToType(const std::string& typeName);

    bool canConvert(VariantType t) const;
    bool canConvert(int t) const;
    static bool canConvert(VariantType t, VariantType s);
    static bool canConvert(VariantType t, int s);
    static bool canConvert(int t, VariantType s);
    static bool canConvert(int t, int s);

    bool getBool() const;
    double getDouble() const;
    float getFloat() const;
    int getInt() const;
    long getLong() const;   
    std::string getString() const;    
    ivec2 getIVec2() const;
    ivec3 getIVec3() const;
    ivec4 getIVec4() const;
    vec2 getVec2() const;
    vec3 getVec3() const;
    vec4 getVec4() const;
    dvec2 getDVec2() const;
    dvec3 getDVec3() const;
    dvec4 getDVec4() const;
    mat2 getMat2() const;
    mat3 getMat3() const;
    mat4 getMat4() const;    

    
    void setBool(const bool& value);
    void setDouble(const double& value);
    void setFloat(const float& value);
    void setInt(const int& value);
    void setLong(const long& value);
    void setString(const std::string& value);
    void setIVec2(const ivec2& value);
    void setIVec3(const ivec3& value);
    void setIVec4(const ivec4& value);
    void setVec2(const vec2& value);
    void setVec3(const vec3& value);
    void setVec4(const vec4& value);
    void setDVec2(const dvec2& value);
    void setDVec3(const dvec3& value);
    void setDVec4(const dvec4& value);
    void setMat2(const mat2& value);
    void setMat3(const mat3& value);
    void setMat4(const mat4& value);  

    template<class T>
    inline void set(const T& value, VariantType type);

    template<class T>
    inline void set(const T& value, int type);

    template<class T>
    inline T get() const;

    void serialize(IvwSerializer& s) const;
    void deserialize(IvwDeserializer& d);

    Variant& operator=(const Variant& rhs);
    Variant& operator=(const bool& rhs);
    Variant& operator=(const double& rhs);
    Variant& operator=(const float& rhs);
    Variant& operator=(const int& rhs);
    Variant& operator=(const long& rhs);
    Variant& operator=(const char* rhs);
    Variant& operator=(const std::string& rhs);
    Variant& operator=(const ivec2& rhs);
    Variant& operator=(const ivec3& rhs);
    Variant& operator=(const ivec4& rhs);
    Variant& operator=(const vec2& rhs);
    Variant& operator=(const vec3& rhs);
    Variant& operator=(const vec4& rhs);
    Variant& operator=(const dvec2& rhs);
    Variant& operator=(const dvec3& rhs);
    Variant& operator=(const dvec4& rhs);
    Variant& operator=(const mat2& rhs);
    Variant& operator=(const mat3& rhs);
    Variant& operator=(const mat4& rhs);
    bool operator==(const Variant& rhs) const;
    bool operator!=(const Variant& rhs) const;

protected:
    void deleteValue();
    template<class T>
    const std::string toString(const T& value) const;
    void* value_;
    VariantType currentType_;
};


#define VP(a) (*(a*)value_)

template<class T>
void Variant::set(const T& value, VariantType type) {
    if (type != currentType_) {
        deleteValue();
        currentType_ = type;
    }
    if (value_ == 0) {
        value_ = new T;
    }
    *(T*)value_ = value;
}

template<class T>
void Variant::set(const T& value, int type) {
    set<T>(value, VariantType(type));
}

template<class T>
T Variant::get() const {
    return VP(T);
}

}
#endif

#ifndef  IVW_MATRIXPROPERTIES_H
#define IVW_MATRIXPROPERTIES_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/properties/ordinalproperty.h"

namespace inviwo {
/** class FloatMat2Property
*  \brief  The FloatMat2Property provides a property holding a 2 by 2 matrix of float values
*  
*  The FlaotMat2Property are used by the processors.
*  It has a widget that shows editable text fields for each element in the matrix in a 2 by 2 grid
* 
* @see OrdinalProperty
*/
class IVW_CORE_API FloatMat2Property : public OrdinalProperty<mat2> {
public:
	/** 
	 * \brief Constructs a FloatMat2Property
	 *
	 * <DESCRIBE THE METHOD>
	 * 
	 * @param std::string identifier <DESCRIBE ME>
	 * @param std::string displayName <DESCRIBE ME>
	 * @param mat2 value <DESCRIBE ME>
	 * @param mat2 minValue <DESCRIBE ME>
	 * @param mat2 maxValue <DESCRIBE ME>
	 * @param mat2 increment <DESCRIBE ME>
	 * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
	 * @param PropertySemantics::Type semantics <DESCRIBE ME>
	 * @return  <DESCRIBE ME>
	 */FloatMat2Property(std::string identifier, std::string displayName,mat2 value,
                      mat2 minValue=mat2(0.0f), mat2 maxValue= mat2(1.0f), mat2 increment=mat2(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatMat2Property"; }
	};
/** class FloatMat3Property
*  \brief  The FloatMat3Property provides a property holding a 3 by 3 matrix of float values
*  
*  The FlaotMat3Property are used by the processors.
*  It has a widget that shows editable text fields for each element in the matrix in a 3 by 3 grid

* @see OrdinalProperty
*/
class IVW_CORE_API FloatMat3Property : public OrdinalProperty<mat3> {
public:
    /** 
     * \brief Constructs a FloatMat3Property
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param mat3 value <DESCRIBE ME>
     * @param mat3 minValue <DESCRIBE ME>
     * @param mat3 maxValue <DESCRIBE ME>
     * @param mat3 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics::Type semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */FloatMat3Property(std::string identifier, std::string displayName,mat3 value,
                      mat3 minValue=mat3(0.0f), mat3 maxValue= mat3(1.0f), mat3 increment=mat3(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatMat3Property"; }
};
/** class FloatMat4Property
*  \brief  The FloatMat4Property provides a property holding a 4 by 4 matrix of float values
*  
*  The FlaotMat4Property are used by the processors.
*  It has a widget that shows editable text fields for each element in the matrix in a 4 by 4 grid
* 
* @see OrdinalProperty
*/
class IVW_CORE_API FloatMat4Property : public OrdinalProperty<mat4> {
public:
    /** 
     * \brief Constructs a FloatMat4Property
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param mat4 value <DESCRIBE ME>
     * @param mat4 minValue <DESCRIBE ME>
     * @param mat4 maxValue <DESCRIBE ME>
     * @param mat4 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics::Type semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */FloatMat4Property(std::string identifier, std::string displayName, mat4 value,
                      mat4 minValue=mat4(0.0f), mat4 maxValue= mat4(1.0f), mat4 increment=mat4(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatMat4Property"; }
};
 
}//namespace

#endif
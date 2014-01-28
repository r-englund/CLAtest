/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_PROPERTYSEMANTICS_H
#define IVW_PROPERTYSEMANTICS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <string>
#include <ostream>

namespace inviwo {
   
class IVW_CORE_API PropertySemantics {
    
public:
    PropertySemantics();
    PropertySemantics(std::string semantic);
    PropertySemantics(const PropertySemantics& rhs);
    PropertySemantics& operator=(const PropertySemantics& that);
    virtual ~PropertySemantics(){}
    
    const std::string& getString() const;
    
    friend std::ostream& operator << (std::ostream& os, const inviwo::PropertySemantics& obj);
    
    static const PropertySemantics Default;

    static const PropertySemantics Color;
    static const PropertySemantics LightPosition;
    static const PropertySemantics TextEditor;
    static const PropertySemantics ImageEditor;
    static const PropertySemantics ShaderEditor;
    
private:
    std::string semantic_;
};

 
inline bool operator==(const PropertySemantics& lhs, const PropertySemantics& rhs){
    return lhs.getString() == rhs.getString();
}
inline bool operator< (const PropertySemantics& lhs, const PropertySemantics& rhs){
    return lhs.getString() < rhs.getString();
}
inline bool operator!=(const PropertySemantics& lhs, const PropertySemantics& rhs){return !operator==(lhs,rhs);}
inline bool operator> (const PropertySemantics& lhs, const PropertySemantics& rhs){return  operator< (rhs,lhs);}
inline bool operator<=(const PropertySemantics& lhs, const PropertySemantics& rhs){return !operator> (lhs,rhs);}
inline bool operator>=(const PropertySemantics& lhs, const PropertySemantics& rhs){return !operator< (lhs,rhs);}

} //Namespace

#endif //IVW_PROPERTYSEMANTICS_H
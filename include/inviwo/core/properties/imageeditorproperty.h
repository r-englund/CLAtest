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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_IMAGEEDITORPROPERTY_H
#define IVW_IMAGEEDITORPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/fileproperty.h>

namespace inviwo {

class IVW_CORE_API ImageLabel : public IvwSerializable {
public :
    ImageLabel();
    ImageLabel(vec2 startPoint, vec2 rectSize, std::string name);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    std::string getName() const {return name_;}
    vec2 getTopLeft() const {return startPoint_;}
    vec2 getSize() const {return rectSize_;}
private:
    std::string name_;
    vec2 startPoint_;
    vec2 rectSize_;
};

/** class ImageEditorProperty
*  A class for file representations.
*  Holds the value of the path to a file as a string.
*  
* @see FileProperty
*/

class IVW_CORE_API ImageEditorProperty : public FileProperty {

public:    
    /** 
     * \brief Constructor for the ImageFileProperty
     *
     * The PropertySemantics can be set to Editor. Then a ImageEditorWidget will be used instead of a FilePropertyWidget
     * 
     * @param std::string identifier identifier for the property 
     * @param std::string displayName displayName for the property
     * @param std::string value the path to the file
     * @param PropertySemantics::Type semantics Can be set to Editor
     */
    ImageEditorProperty(std::string identifier, std::string displayName, std::string value,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT, 
                 PropertySemantics::Type semantics = PropertySemantics::Editor);
    virtual std::string getClassName()  const { return "ImageEditorProperty"; }
    void addLabel(vec2 start, vec2 end, std::string name="");
    void setDimension(ivec2 imgSize);
    std::vector<ImageLabel*> getLabels() const;
    void clearLabels();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);    
private:
    std::vector<ImageLabel*> labels_;
    ivec2 dimensions_;
};

} // namespace

#endif // IVW_IMAGEEDITORPROPERTY_H
/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_FILEPROPERTY_H
#define IVW_FILEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {
/** class FileProperty
*  A class for file representations.
*  Holds the value of the path to a file as a string.
*  
* @see TemplateProperty
*/

class IVW_CORE_API FileProperty : public TemplateProperty<std::string> {

public:

    enum AcceptMode { AcceptOpen, AcceptSave };
    enum FileMode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly };
    
    /** 
     * \brief Constructor for the FileProperty
     *
     * The PropertySemantics can be set to TextEditor. Then a TextEditorWidget will be used instead of a FilePropertyWidget
     * 
     * @param std::string identifier identifier for the property 
     * @param std::string displayName displayName for the property
     * @param std::string value the path to the file
     * @param PropertySemantics semantics Can be set to Editor
     */
    FileProperty(std::string identifier, std::string displayName, std::string value="",
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT, 
                 PropertySemantics semantics = PropertySemantics::Default);
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FileProperty"; }

    virtual void addNameFilter(std::string);
    virtual void clearNameFilters();
    virtual std::vector<std::string> getNameFilters();

    virtual void setAcceptMode(AcceptMode mode);
    AcceptMode getAcceptMode() const;
    
    virtual void setFileMode(FileMode mode);
    FileMode getFileMode() const;
    
private:
    std::vector<std::string> nameFilters_;
    AcceptMode acceptMode_;
    FileMode fileMode_;
};

} // namespace

#endif // IVW_FILEPROPERTY_H
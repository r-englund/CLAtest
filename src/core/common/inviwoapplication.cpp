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
 * Primary author : Erik Sundén
 *
 **********************************************************************/


#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/io/datawriterfactory.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/ports/portinspectorfactory.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/processors/processorwidgetfactory.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/propertyfactory.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/rendering/geometryrendererfactory.h>

namespace inviwo {

// TODO: are the first two constructors needed? Otherwise remove.
InviwoApplication::InviwoApplication()
    : displayName_("Inviwo"), basePath_("")
{
    init(this);
}

InviwoApplication::InviwoApplication(std::string displayName, 
                                     std::string basePath)
    : displayName_(displayName)
    , basePath_(basePath) {

    init(this);
}

InviwoApplication::InviwoApplication(int argc, char** argv,
                                     std::string displayName,
                                     std::string basePath)
    : displayName_(displayName)
    , basePath_(basePath) {

    commandLineParser_ = new CommandLineParser(argc, argv);
	commandLineParser_->initialize();
	commandLineParser_->parse();
    init(this);
}

InviwoApplication::~InviwoApplication() {
    if(initialized_)
        deinitialize();
    for (size_t i=0; i<modules_.size(); i++){
        InviwoModule* module = modules_[i];
        delete module;
    }
    modules_.clear();
    delete commandLineParser_;
    SingeltonBase::deleteAllSingeltons();
    DataFormatBase::cleanDataFormatBases();
}

void InviwoApplication::initialize(registerModuleFuncPtr regModuleFunc) {
    printApplicationInfo();

    // initialize singleton factories
    DataReaderFactory::init();
    DataWriterFactory::init();
    GeometryRendererFactory::init();
    MetaDataFactory::init();
    PortInspectorFactory::init();
    ProcessorFactory::init();
    ProcessorWidgetFactory::init(); 
    PropertyFactory::init();
    PropertyWidgetFactory::init();
    RepresentationConverterFactory::init();

    registerModule(new InviwoCore());
    (*regModuleFunc)(this);

    for(size_t i = 0; i < modules_.size(); i++) {
        modules_[i]->initialize();
    }

    initialized_ = true;
}

void InviwoApplication::deinitialize() {
    for (size_t i=0; i<modules_.size(); i++)
        modules_[i]->deinitialize();
    initialized_ = false;
}

std::string InviwoApplication::getPath(PathType pathType, const std::string& suffix) {
    std::string result = getBasePath();
    switch (pathType) {
    case inviwo::InviwoApplication::PATH_DATA:
        result += "data/";
        break;
    case inviwo::InviwoApplication::PATH_VOLUMES:
        result += "data/volumes/";
        break;
    case inviwo::InviwoApplication::PATH_MODULES:
        result += "modules/";
        break;
    case inviwo::InviwoApplication::PATH_WORKSPACES:
        result += "data/workspaces/";
        break;
    case inviwo::InviwoApplication::PATH_IMAGES:
        result += "data/images/";
        break;
    case inviwo::InviwoApplication::PATH_RESOURCES:
        result += "resources";
        break;
    case inviwo::InviwoApplication::PATH_TRANSFERFUNCTIONS:
        result += "data/transferfunctions/";
        break;
    default:
        break;
    }
    result += suffix;
    return result;
}

void InviwoApplication::printApplicationInfo(){
    LogInfoCustom("InviwoInfo", "Inviwo Version: " << IVW_VERSION);
    std::string config = "";
#ifdef CMAKE_GENERATOR
    config += std::string(CMAKE_GENERATOR);
#endif
#if defined(CMAKE_BUILD_TYPE)
    config += " [" + std::string(CMAKE_BUILD_TYPE) + "]";
#elif defined(CMAKE_INTDIR)
    config += " [" + std::string(CMAKE_INTDIR) + "]";
#endif
    if (config != "")
        LogInfoCustom("InviwoInfo", "Config: " << config);
}

void InviwoApplication::addCallbackAction(ModuleCallbackAction* callbackAction) {
    moudleCallbackActions_.push_back(callbackAction);
}

std::vector<ModuleCallbackAction*> InviwoApplication::getCallbackActions() {
    return moudleCallbackActions_;
}

std::vector<Settings*> InviwoApplication::getModuleSettings() {
    std::vector<Settings*> allModuleSettings;
    for (size_t i=0; i<modules_.size(); i++) {
        const std::vector<Settings*> modSettings = modules_[i]->getSettings();
        allModuleSettings.insert(allModuleSettings.end(), modSettings.begin(), modSettings.end());

    }
    return allModuleSettings;
}


} // namespace

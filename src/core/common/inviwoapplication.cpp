/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/util/dialogfactory.h>
#include <inviwo/core/io/datawriterfactory.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/ports/portfactory.h>
#include <inviwo/core/ports/portinspectorfactory.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/processors/processorwidgetfactory.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/propertyfactory.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <inviwo/core/resources/resourcemanager.h>

namespace inviwo {

// TODO: are the first two constructors needed? Otherwise remove.
InviwoApplication::InviwoApplication()
    : displayName_("Inviwo")
    , basePath_("")
    , commandLineParser_(NULL){
    init(this);
}

InviwoApplication::InviwoApplication(std::string displayName,
                                     std::string basePath)
    : displayName_(displayName)
    , basePath_(basePath)
    , commandLineParser_(NULL) {
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
    if (initialized_)
        deinitialize();

    for (size_t i=0; i<modules_.size(); i++) {
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
    // initialize singletons 
    ResourceManager::init();
    DataReaderFactory::init();
    DataWriterFactory::init();
    DialogFactory::init();
    GeometryRendererFactory::init();
    MetaDataFactory::init();
    PortFactory::init();
    PortInspectorFactory::init();
    ProcessorFactory::init();
    ProcessorWidgetFactory::init();
    PropertyFactory::init();
    PropertyWidgetFactory::init();
    RepresentationConverterFactory::init();
    registerModule(new InviwoCore());
    (*regModuleFunc)(this);

    for (size_t i = 0; i < modules_.size(); i++)
        modules_[i]->initialize();

    initialized_ = true;
}

void InviwoApplication::deinitialize() {
    // Deinitialize Resource manager before modules
    // to prevent them from using module specific 
    // (OpenGL/OpenCL) features after their module 
    // has been deinitialized
    ResourceManager::deleteInstance();
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

        case inviwo::InviwoApplication::PATH_SETTINGS:
            result += "data/settings/";
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

void InviwoApplication::printApplicationInfo() {
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

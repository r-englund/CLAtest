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
 * Main file authors: Peter Steneteg, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_INVIWOMODULE_H
#define IVW_INVIWOMODULE_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/io/datareader.h>
#include <inviwo/core/io/datareaderdialog.h>
#include <inviwo/core/io/datawriter.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/ports/port.h>
#include <inviwo/core/ports/portfactoryobject.h>
#include <inviwo/core/ports/portinspector.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/processorfactoryobject.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/propertyfactoryobject.h>
#include <inviwo/core/properties/propertywidgetfactoryobject.h>
#include <inviwo/core/rendering/geometryrenderer.h>
#include <inviwo/core/resources/resource.h>
#include <inviwo/core/util/capabilities.h>
#include <inviwo/core/util/dialogfactoryobject.h>


namespace inviwo {

class Settings;

/**
 * \class InviwoModule
 *
 * \brief A module class contains registrations of functionality, such as processors, ports, properties etc.
 */
class IVW_CORE_API InviwoModule {

public:
    InviwoModule();
    virtual ~InviwoModule();

    std::string getIdentifier() const;

    bool isInitialized() const;

    const std::vector<Capabilities*>& getCapabilities() const;
    const std::vector<Data*>& getData() const;
    const std::vector<DataReader*>& getDataReaders() const;
    const std::vector<DataRepresentation*>& getDataRepresentations() const;
    const std::vector<DataWriter*>& getDataWriters() const;
    const std::vector<DialogFactoryObject*>& getDialogs() const;
    const std::vector<MetaData*>& getMetaData() const;
    const std::vector<PortFactoryObject*>& getPorts() const;
    const std::vector<PortInspector*>& getPortInspectors() const;
    const std::vector<ProcessorFactoryObject*>& getProcessors() const;
    const std::vector< std::pair<std::string, ProcessorWidget*> >& getProcessorWidgets() const;
    const std::vector<PropertyFactoryObject*>& getProperties() const;
    const std::vector<PropertyWidgetFactoryObject*>& getPropertyWidgets() const;
    const std::vector<GeometryRenderer*>& getRenderers() const;
    const std::vector<RepresentationConverter*>& getRepresentationConverters() const;
    const std::vector<Resource*>& getResources() const;
    const std::vector<Settings*>& getSettings() const;

    std::string getDescription() const;
    void setDescription(const std::string& description) const;

    virtual void initialize();
    virtual void deinitialize();

protected:
    void setIdentifier(const std::string& identifier);

    void registerCapabilities(Capabilities* info);
    void registerData(Data* data);
    void registerDataReader(DataReader* reader);
    void registerDataRepresentation(DataRepresentation* dataRepresentation);
    void registerDataWriter(DataWriter* writer);
    void registerDialogObject(DialogFactoryObject* dialog);
    void registerMetaData(MetaData* meta);
    void registerPortObject(PortFactoryObject* port);
    void registerPortInspector(PortInspector* portInspector);
    void registerProcessorObject(ProcessorFactoryObject* processor);
    void registerProcessorWidget(std::string processorClassName, ProcessorWidget* processorWidget);
    void registerPropertyObject(PropertyFactoryObject* property);
    void registerPropertyWidgetObject(PropertyWidgetFactoryObject* property);
    void registerRenderer(GeometryRenderer* renderer);
    void registerRepresentationConverter(RepresentationConverter* representationConverter);
    void registerResource(Resource* resource);
    void registerSettings(Settings* settings);

    /**
     * Initializes all settings registered by the module.
     * @note Call InviwoModule::setupModuleSettings() if overriding this function
     */
    virtual void setupModuleSettings();

    std::string getPath(const std::string& suffix = "") const;

    std::vector<Settings*> moduleSettings_;

private:
    std::string identifier_;

    bool initialized_;

    std::vector<Capabilities*> capabilities_;
    std::vector<Data*> data_;
    std::vector<DataRepresentation*> dataRepresentations_;
    std::vector<DataReader*> dataReaders_;
    std::vector<DataWriter*> dataWriters_;
    std::vector<DialogFactoryObject*> dialogs_;
    std::vector<MetaData*> metadata_;
    std::vector<PortFactoryObject*> ports_;
    std::vector<PortInspector*> portInspectors_;
    std::vector<ProcessorFactoryObject*> processors_;
    std::vector<std::pair<std::string, ProcessorWidget*> > processorWidgets_;
    std::vector<PropertyFactoryObject*> properties_;
    std::vector<PropertyWidgetFactoryObject*> propertyWidgets_;
    std::vector<GeometryRenderer*> renderers_;
    std::vector<RepresentationConverter*> representationConverters_;
    std::vector<Resource*> resources_;
};


#define registerProcessor(T) { registerProcessorObject(new ProcessorFactoryObjectTemplate<T>()); }
#define registerProperty(T) { registerPropertyObject(new PropertyFactoryObjectTemplate<T>(#T)); }
#define registerPropertyWidget(T, P, semantics) { registerPropertyWidgetObject(new PropertyWidgetFactoryObjectTemplate<T,P>(#P, PropertySemantics(semantics))); }
#define registerPort(T) { registerPortObject(new PortFactoryObjectTemplate<T>(#T)); }
#define registerDialog(P, T) { registerDialogObject(new DialogFactoryObjectTemplate<T>(P)); }


} // namespace

#endif // IVW_INVIWOMODULE_H

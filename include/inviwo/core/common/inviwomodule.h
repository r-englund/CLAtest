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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

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

namespace inviwo {

class Settings;

class IVW_CORE_API InviwoModule {

public:
    InviwoModule();
    virtual ~InviwoModule();

    std::string getIdentifier() const;

    bool isInitialized() const;

    const std::vector<Capabilities*>& getCapabilities() const;
    const std::vector<Data*>& getData() const;
    const std::vector<DataReader*>& getDataReaders() const;
    const std::vector<DataReaderDialog*>& getDataReaderDialogs() const;
    const std::vector<DataRepresentation*>& getDataRepresentations() const;
    const std::vector<DataWriter*>& getDataWriters() const;
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
    void registerDataReaderDialog(DataReader* reader, DataReaderDialog* readerDialog);
    void registerDataRepresentation(DataRepresentation* dataRepresentation);
    void registerDataWriter(DataWriter* writer);
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
    
    void setXMLFileName(const std::string& xmlDocuFileName);

    virtual void setupModuleSettings(){};

    std::string getPath(const std::string& suffix = "") const;

    std::vector<Settings*> moduleSettings_;

private:
    std::string identifier_;

    bool initialized_;

    std::vector<Capabilities*> capabilities_;
    std::vector<Data*> data_;
    std::vector<DataRepresentation*> dataRepresentations_;
    std::vector<DataReader*> dataReaders_;
    std::vector<DataReaderDialog*> dataReaderDialogs_;
    std::vector<DataWriter*> dataWriters_;
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

    std::string xmlDocumentFileName_;
};


#define registerProcessor(T) { registerProcessorObject(new ProcessorFactoryObjectTemplate<T>(T::CLASS_NAME, T::CATEGORY, T::CODE_STATE)); }
#define registerProperty(T) { registerPropertyObject(new PropertyFactoryObjectTemplate<T>(#T)); }
#define registerPropertyWidget(T, P, semantics) { registerPropertyWidgetObject(new PropertyWidgetFactoryObjectTemplate<T,P>(#P, PropertySemantics(semantics))); }
#define registerPort(T) { registerPortObject(new PortFactoryObjectTemplate<T>(#T)); }


} // namespace

#endif // IVW_INVIWOMODULE_H

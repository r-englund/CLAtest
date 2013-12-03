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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/network/portconnection.h>

namespace inviwo {

PortConnection::PortConnection()   
    : inport_(0),
      outport_(0){}

PortConnection::PortConnection(Outport* outport, Inport* inport)
    : inport_(inport),
      outport_(outport){}

PortConnection::~PortConnection() {}

void PortConnection::serialize(IvwSerializer& s) const {  
    s.serialize("OutPort", *getOutport());
    s.serialize("InPort", *getInport());
}

void PortConnection::deserialize(IvwDeserializer& d) {    
    Outport outport("");
    d.deserialize("OutPort", outport);
    Processor* outPortProcessor = outport.getProcessor();
    ivwAssert(outPortProcessor!=0, "Unable to load processor.")
    
    if (outPortProcessor)
        outport_ = outPortProcessor->getOutport(outport.getIdentifier());
    
    Inport inport("");
    d.deserialize("InPort", inport);
    Processor* inPortProcessor = inport.getProcessor();
    ivwAssert(outPortProcessor!=0, "Unable to load processor.")

    if (inPortProcessor)
        inport_ = inPortProcessor->getInport(inport.getIdentifier());
}

} // namespace

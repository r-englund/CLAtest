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
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#include "../pythoninterface/pymethod.h"

#include "pymodule.h"

namespace inviwo{


PyMethod::PyMethod(){
}


PyMethodDef* PyMethod::getDef(){
    def_.ml_doc   = getDesc();
    def_.ml_flags = getFlags();
    def_.ml_meth  = getFunc();
    def_.ml_name  = getName();
    return &def_;
}




}//namespace
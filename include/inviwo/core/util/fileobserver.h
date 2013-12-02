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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_FILEOBSERVER_H
#define IVW_FILEOBSERVER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <string>
#include <vector>
#include <utility>

namespace inviwo {

class IVW_CORE_API FileObserver {

public:
    FileObserver();
    ~FileObserver();

    void startFileObservation(std::string fileName);
    void stopFileObservation(std::string fileName);
    std::vector<std::string> getFiles();

    bool isObserved(std::string fileName);
    void increaseNumObservers(std::string fileName);
    void decreaseNumObservers(std::string fileName);
    int getNumObservers(std::string fileName);

    virtual void fileChanged(std::string fileName) = 0;

private:
    std::vector<std::pair<std::string, int> >* observedFiles_; ///< stores the files to be observed
                                                              ///< plus the number of observers for each
};

} // namespace

#endif // IVW_FILEOBSERVER_H

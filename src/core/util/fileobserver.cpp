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

#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

FileObserver::FileObserver(){
  //  observedFiles_ = new std::vector<std::pair<std::string, int> >();
}

FileObserver::~FileObserver(){
//    delete observedFiles_;
}

void FileObserver::startFileObservation(std::string fileName) {
    if (isObserved(fileName))
        increaseNumObservers(fileName);
    else {
        if( URLParser::fileExists(fileName) ) {
            observedFiles_.push_back(std::pair<std::string,int>(fileName, 1));
            InviwoApplication::getRef().startFileObservation(fileName);
        }
    }
}

void FileObserver::stopFileObservation(std::string fileName) {
    if (isObserved(fileName)) {
        if (getNumObservers(fileName) > 1)
            decreaseNumObservers(fileName);
        else {
            observedFiles_.erase(std::remove(observedFiles_.begin(), observedFiles_.end(), std::pair<std::string,int>(fileName,getNumObservers(fileName))), observedFiles_.end());
            InviwoApplication::getRef().stopFileObservation(fileName);
        }
    }
}

void FileObserver::increaseNumObservers(std::string fileName) {
    for (size_t i=0;i<observedFiles_.size();i++)
        if (observedFiles_.at(i).first == fileName)
            observedFiles_.at(i).second++;
}

void FileObserver::decreaseNumObservers(std::string fileName) {
    for (size_t i=0;i<observedFiles_.size();i++)
        if (observedFiles_.at(i).first == fileName)
            observedFiles_.at(i).second--;
}

int FileObserver::getNumObservers(std::string fileName) {
    for (size_t i=0;i<observedFiles_.size();i++)
        if (observedFiles_.at(i).first == fileName)
            return observedFiles_.at(i).second;
    return 0;
}

bool FileObserver::isObserved(std::string fileName) {
    return (getNumObservers(fileName) > 0);
}

std::vector<std::string> FileObserver::getFiles() {
    std::vector<std::string> files;
    for (size_t i=0;i<observedFiles_.size();i++)
        files.push_back(observedFiles_.at(i).first);
    return files;
}

} // namespace
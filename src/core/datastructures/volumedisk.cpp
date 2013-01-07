#include "inviwo/core/datastructures/volumedisk.h"

namespace inviwo {

    VolumeDisk::VolumeDisk(std::string srcFile)
        : VolumeRepresentation(ivec3(128,128,128), "UINT8")
    {
        sourceFile_ = srcFile;
        initialize();
    }

    VolumeDisk::~VolumeDisk() {}

    void VolumeDisk::initialize() {
        // From url
        //1. read file name 
        //2. read format
        //3. read dimension
        
        rawFile_ = sourceFile_;
        dataFormat_ = "UINT8";
        dimensions_ = ivec3(128,128,128);

        size_t pos = sourceFile_.find_last_of("\\/") + 1;
        sourceFileLocation_ = sourceFile_.substr(0, pos);

        pos = sourceFile_.rfind(".") + 1;
        sourceFileExtension_ = sourceFile_.substr( pos, sourceFile_.length());

        if (sourceFileExtension_=="dat") {
            //TODO: use dat file reader
            std::istream* f = new std::ifstream(sourceFile_.c_str());

            std::string textLine; 
            getline(*f, textLine);

            std::string key;
            do {               
                std::stringstream ss(textLine);
                transform(textLine.begin(), textLine.end(), textLine.begin(), (int (*)(int))tolower);
                ss >> key;            
                if (key=="ObjectFileName:") {
                    ss >> rawFile_;
                    rawFile_ = sourceFileLocation_ + rawFile_;
                }
                else if (key=="Resolution:") {
                    ss >> dimensions_.x;
                    ss >> dimensions_.y;
                    ss >> dimensions_.z;
                }
                else if (key=="Format:") {
                    ss >> dataFormat_;

                    if (dataFormat_=="UCHAR") {
                        dataFormat_ = "UINT8";
                    }
                    else if (dataFormat_=="USHORT") {
                        dataFormat_ = "UINT16";
                    }
                    else
                        dataFormat_="";
                }
                getline(*f, textLine);
            }while (!f->eof());  
        }

    }

    void VolumeDisk::deinitialize() {

    }

    DataRepresentation* VolumeDisk::clone() {
        return new VolumeDisk(sourceFile_);
    }



} // namespace

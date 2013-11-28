#include <inviwo/core/io/datareader.h>

namespace inviwo {

    DataReader::DataReader() : extensions_() {}

    DataReader::DataReader( const DataReader& rhs ) : extensions_(rhs.extensions_){    
    }

    DataReader& DataReader::operator=( const DataReader& that ){
        if (this != &that) {
            extensions_.clear();
            for(std::vector<FileExtension>::const_iterator it = that.getExtensions().begin();
                it != that.getExtensions().end(); ++it){
                    extensions_.push_back(*it);
            }
        }
        return *this;
    }

    const std::vector<FileExtension>& DataReader::getExtensions() const {
        return extensions_;
    }
    void DataReader::addExtension( FileExtension ext ){
        extensions_.push_back(ext);
    }



} // namespace

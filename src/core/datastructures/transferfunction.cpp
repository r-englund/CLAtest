#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {
    TransferFunction::TransferFunction() {
        data_ = new Image(uvec2(256,1), DataVec4FLOAT32());
    }

	TransferFunction::TransferFunction(const TransferFunction& rhs) {
        data_ = NULL;
		*this = rhs;
	}

	TransferFunction& TransferFunction::operator=(const TransferFunction& rhs) {
		if (this != &rhs) {
            delete data_;
			this->data_ = static_cast<Image*>(rhs.data_->clone());
			this->clearPoints();
			
			for (size_t i = 0; i < rhs.getNumberOfDataPoints(); ++i){
				this->dataPoints_.push_back(new TransferFunctionDataPoint(*rhs.getPoint(i)));
			}
		}
		this->calcTransferValues();
		return *this;
	}

    TransferFunction::~TransferFunction(){
		clearPoints();
        delete data_;
	}

    Image* TransferFunction::getData() const{
		return data_;
    }

    size_t TransferFunction::getNumberOfDataPoints() const{
        return dataPoints_.size();
    }
    TransferFunctionDataPoint* TransferFunction::getPoint(int i) const{
		if (getNumberOfDataPoints() == 0){
			return NULL;
		}
		return dataPoints_[i];
    }

	void TransferFunction::addPoint(vec2* pos, vec4* rgba){
		this->addPoint(new TransferFunctionDataPoint(pos, rgba));
	}

	void TransferFunction::addPoint(vec2 pos, vec4 rgba){
		this->addPoint(new TransferFunctionDataPoint(pos, rgba));
	}

    void TransferFunction::addPoint(TransferFunctionDataPoint* newPoint){
        std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin();

        float pointpos = newPoint->getPos()->x;
        float iterpos;

        if (dataPoints_.size() == 0){
            dataPoints_.push_back(newPoint);
        }
        else{
            if (pointpos > dataPoints_.back()->getPos()->x){
                dataPoints_.push_back(newPoint);
            }
            else{
                for (iter = dataPoints_.begin(); iter != dataPoints_.end(); iter++){
                    iterpos = (*iter)->getPos()->x;
                    if (iterpos > pointpos){
                        dataPoints_.insert(iter, newPoint);
                        break;
                    }
                }
            }
        }
        calcTransferValues();
    }


    void TransferFunction::removePoint(TransferFunctionDataPoint* p){
        std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin();
		for (iter = dataPoints_.begin(); iter != dataPoints_.end(); iter++){
            if ((*iter) == p){
                dataPoints_.erase(iter);
                break;
            }
        }
        calcTransferValues();
    }

	void TransferFunction::clearPoints(){
		if (dataPoints_.size() > 0){

			for (std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin(); iter != dataPoints_.end(); iter++){
				//std::cout << "XPOS: " << (*iter)->getPos()->x << std::endl;
			}

			for (std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin(); iter != dataPoints_.end();){
				delete *iter;
				iter = dataPoints_.erase(iter);
			}
			dataPoints_.clear();
		}
	}

    void TransferFunction::calcTransferValues(){
		//std::cout << "POINTS: " << dataPoints_.size() << std::endl;

        vec4* dataArray = static_cast<vec4*>(data_->getEditableRepresentation<ImageRAM>()->getData());
        float factor;
        float start;
        float stop;
        const vec4* startValues;
        const vec4* stopValues;

        if ((int)dataPoints_.size() == 0){
			for (int i = 0; i < 256 ; i++){
				dataArray[i] = vec4(i/255.0f, i/255.0f, i/255.0f, 1.0f);
			}
		}

        else if ((int)dataPoints_.size () == 1){
            for (int i = 0; i < 256 ; i++){
                dataArray[i] = *dataPoints_[0]->getRgba();
            }
        }
        else{
			//This loop fills out the values before the first point
            for (int i = 0; i < (int)dataPoints_.front()->getPos()->x ; i++){
                dataArray[i] = *dataPoints_.front()->getRgba();
            }
            //Loops through all point to point intervals
            for (size_t i = 0; i < getNumberOfDataPoints() - 1; i++){
                start = dataPoints_[i]->getPos()->x;
                stop = dataPoints_[i + 1]->getPos()->x;

                startValues = dataPoints_[i]->getRgba();
                stopValues = dataPoints_[i + 1]->getRgba();

                //Interpolates the function values for all intermediate positions
                for (int j = (int)start; j <=  (int)stop; j++){
                    factor = (j - start)/(stop - start);
					dataArray[j] = (*startValues) * (1.0f-factor) + factor * (*stopValues);
                }
            }

			//This loop fills out the values after the last point
            for (int i = (int)dataPoints_.back()->getPos()->x; i < 256; i++){
                dataArray[i] = *dataPoints_.back()->getRgba();
            }
        }
    }


    bool myPointCompare (TransferFunctionDataPoint* a, TransferFunctionDataPoint* b){
        return a->getPos()->x < b->getPos()->x;
    }

    void TransferFunction::sortDataPoints(){
        std::sort(dataPoints_.begin(), dataPoints_.end(), myPointCompare);
    }
};
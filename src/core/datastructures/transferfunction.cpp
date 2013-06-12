#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {
    TransferFunction::TransferFunction(){
        data_ = new Image();
        data_->addRepresentation(new ImageRAMVec4float32 (uvec2(256,1)));
    }

    TransferFunction::~TransferFunction(){}

    Image* TransferFunction::getData() const{
        return const_cast<Image*>(data_);
    }

    int TransferFunction::getSize(){
        return dataPoints_.size();
    }
    TransferFunctionDataPoint* TransferFunction::getPoint(int i){
        return dataPoints_[i];
    }

    void TransferFunction::addPoint(vec2* pos, vec4* rgba){
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
            if (p->getPos()->x == (*iter)->getPos()->x && p->getPos()->y == (*iter)->getPos()->y){
                dataPoints_.erase(iter);
                break;
            }
        }
        calcTransferValues();
    }

    void TransferFunction::calcTransferValues(){
        vec4* dataArray = static_cast<vec4*>(data_->getEditableRepresentation<ImageRAMVec4float32>()->getData());
        float factor;
        float start;
        float stop;
        const vec4* startValues;
        const vec4* stopValues;

        if ((int)dataPoints_.size() == 0){} 
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
            for (int i = 0; i < getSize() - 1; i++){
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


    bool myPointCompare (TransferFunctionDataPoint * a, TransferFunctionDataPoint* b){
        return a->getPos()->x < b->getPos()->x;
    }

    void TransferFunction::sortDataPoints(){
        std::sort(dataPoints_.begin(), dataPoints_.end(), myPointCompare);
    }
};
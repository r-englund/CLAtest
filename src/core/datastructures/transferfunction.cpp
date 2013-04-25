#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {

    TransferFunction::TransferFunction(){
        //data_.addRepresentation(new ImageRAMfloat32(uvec2(256,1)));
        data_.addRepresentation(new ImageRAMVec4float32 (uvec2(256,1)));
        pointPositions_.push_back(new vec2(0.0f, 0.0f));
        pointPositions_.push_back(new vec2(255.0f, 100.0f));
        pointValues_.push_back(new vec4(0.0f, 0.0f, 0.0f, 1.0f));
        pointValues_.push_back(new vec4(1.0f, 0.0f, 0.0f, 1.0f));

        dataArray_ = static_cast<vec4*>(data_.getEditableRepresentation<ImageRAMVec4float32>()->getData());
        //dataArray_ = static_cast<vec4*>(imgRam->getData());
    }

    TransferFunction::~TransferFunction() {
    }

    void TransferFunction::setData(Image data){
        data_ = data;
    }

    Image* TransferFunction::getData() const{
        return const_cast<Image*>(&data_);
    }

    vec2* TransferFunction::getPosition(int i){     
        return pointPositions_[i];
    };

    void TransferFunction::setPosition(int i, vec2* val){     
        pointPositions_[i] = val;
    };

    vec4* TransferFunction::getValue(int i){
        return pointValues_[i];
    };

    void TransferFunction::setValue(int i, vec4* val){
        dataArray_[i] = *val;
    };

    void TransferFunction::createPoint(vec2* pos, vec4* val){
        
    }

    void TransferFunction::calcTransferValues(){
        vec4* newValues;
        std::stringstream ss;
        glm::quat startQuat;
        glm::quat stopQuat;
        float factor;

        //Loops through all point to point intervals
        for (int i = 0; i < (int)pointPositions_.size() - 1; i++){

            vec2* start = pointPositions_[i];
            vec2* stop = pointPositions_[i + 1];

            vec4* startValues = pointValues_[i];
            vec4* stopValues = pointValues_[i + 1];

            //Interpolates the function values for all intermediate positions
            for (int j = (int)start->x; j <=  (int)stop->x; j++){
                factor = (j - start->x)/(stop->x - start->x);

                startQuat.x = startValues->a;
                stopQuat.x = stopValues->a;
                float newA = glm::lerp(startQuat, stopQuat, factor).x;
                startQuat.x = startValues->r;
                stopQuat.x = stopValues->r;
                float newR = glm::lerp(startQuat, stopQuat, factor).x; 
                startQuat.x = startValues->b;
                stopQuat.x = stopValues->b;
                float newB = glm::lerp(startQuat, stopQuat, factor).x;
                startQuat.x = startValues->g;
                stopQuat.x = stopValues->g;
                float newG = glm::lerp(startQuat, stopQuat, factor).x;
                newValues = new vec4(newR, newB, newG, newA);
                dataArray_[j] = *newValues;
            }
        }
    }
}
#include <inviwo/core/datastructures/transferfunction.h>
#include <math.h>

namespace inviwo {
TransferFunction::TransferFunction() {
	textureSize_ = 1024;
	data_ = new Image(uvec2(textureSize_, 1), COLOR_ONLY, DataVec4FLOAT32());
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
		textureSize_ = rhs.textureSize_;
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

const Image* TransferFunction::getData() const{
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

	float pointPosition = newPoint->getPos()->x;
	float iterPosition;

	if (dataPoints_.size() == 0){
		dataPoints_.push_back(newPoint);
	}
	else{
		if (pointPosition > dataPoints_.back()->getPos()->x){
			dataPoints_.push_back(newPoint);
		}
		else{
			for (iter = dataPoints_.begin(); iter != dataPoints_.end(); iter++){
				iterPosition = (*iter)->getPos()->x;
				if (iterPosition > pointPosition){
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
		for (std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin(); iter != dataPoints_.end();){
			delete *iter;
			iter = dataPoints_.erase(iter);
		}
		dataPoints_.clear();
	}
}

void TransferFunction::calcTransferValues(){
	//int dataArraySize = (int)pow(2.0f, databits_) - 1;
	int dataArraySize = 1023;
	data_->resize(uvec2(textureSize_, 1));
	vec4* dataArray = static_cast<vec4*>(data_->getEditableRepresentation<ImageRAM>()->getData());

	//In case of 0 points
	if ((int)dataPoints_.size() == 0){
		for (int i = 0; i <= dataArraySize; i++){
			dataArray[i] = vec4((float)i/dataArraySize, (float)i/dataArraySize, (float)i/dataArraySize, 1.0f);
		}
	}
	//In case of 1 point
	else if ((int)dataPoints_.size () == 1){ 
		for (size_t i = 0; i <= (size_t)dataArraySize ; ++i){
			dataArray[i] = *dataPoints_[0]->getRgba();
		}
	}

	//In case of >1 points
	else{
		int frontX = (int)ceil(dataPoints_.front()->getPos()->x * dataArraySize);
		int backX = (int)ceil(dataPoints_.back()->getPos()->x * dataArraySize);

		for (size_t i = 0; i <= (size_t)frontX; ++i){
			dataArray[i] = *dataPoints_.front()->getRgba();
		}
		for (size_t i = backX; i <= (size_t)dataArraySize; ++i){
			dataArray[i] = *dataPoints_.back()->getRgba();
		}

		std::vector<TransferFunctionDataPoint*>::iterator pLeft = dataPoints_.begin();
		std::vector<TransferFunctionDataPoint*>::iterator pRight = dataPoints_.begin() + 1;

		while(pRight != dataPoints_.end()){
			int n = (int)ceil((*pLeft)->getPos()->x * dataArraySize);

			while (n < ceil((*pRight)->getPos()->x * dataArraySize)){
				vec4 lrgba = *(*pLeft)->getRgba();
				vec4 rrgba = *(*pRight)->getRgba();
				float lx = (*pLeft)->getPos()->x * dataArraySize;
				float rx = (*pRight)->getPos()->x * dataArraySize;
				dataArray[n] = (n - lx)/(rx - lx)*(rrgba - lrgba) + lrgba;
				n++;
			}
			pLeft++;
			pRight++;
		}
	}
	//std::cout << data_->getDimension().x << " " << data_->getDimension().y << std::endl;
}

bool myPointCompare (TransferFunctionDataPoint* a, TransferFunctionDataPoint* b){
	return a->getPos()->x < b->getPos()->x;
}

void TransferFunction::sortDataPoints(){
	std::sort(dataPoints_.begin(), dataPoints_.end(), myPointCompare);
}

int TransferFunction::getTextureSize(){
	return textureSize_;
};

};
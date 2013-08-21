#include <inviwo/core/datastructures/transferfunction.h>

namespace inviwo {
TransferFunction::TransferFunction() {
	data_ = new Image(uvec2(256,1), COLOR_ONLY, DataVec4FLOAT32());
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
		}

		for (std::vector<TransferFunctionDataPoint*>::iterator iter = dataPoints_.begin(); iter != dataPoints_.end();){
			delete *iter;
			iter = dataPoints_.erase(iter);
		}
		dataPoints_.clear();
	}
}

void TransferFunction::calcTransferValues(){
	vec4* dataArray = static_cast<vec4*>(data_->getEditableRepresentation<ImageRAM>()->getData());

	int maxValue = 255;

	//In case of 0 points
	if ((int)dataPoints_.size() == 0){
		for (int i = 0; i <= maxValue; i++){
			dataArray[i] = vec4((float)i/maxValue, (float)i/maxValue, (float)i/maxValue, 1.0f);
		}
	}
	//In case of 1 point
	else if ((int)dataPoints_.size () == 1){ 
		//std::fill_n(dataArray, maxValue, *dataPoints_[0]->getRgba());
		for (size_t i = 0; i <= maxValue ; ++i){
			dataArray[i] = *dataPoints_[0]->getRgba();
		}
	}

	//In case of >1 points
	else{
		int firstX = (int)ceil(dataPoints_.front()->getPos()->x * maxValue);
		int lastX = (int)ceil(dataPoints_.back()->getPos()->x * maxValue);

		//std::fill_n(dataArray, firstX, *dataPoints_.front()->getRgba());
		//std::fill_n(dataArray, maxValue, *dataPoints_.back()->getRgba());

		for (size_t i = 0; i <= firstX; ++i){
			dataArray[i] = *dataPoints_.front()->getRgba();
		}

		for (size_t i = lastX; i <= maxValue; ++i){
			dataArray[i] = *dataPoints_.back()->getRgba();
		}

		std::vector<TransferFunctionDataPoint*>::iterator pleft = dataPoints_.begin();
		std::vector<TransferFunctionDataPoint*>::iterator  pright = dataPoints_.begin() + 1;

		while(pright != dataPoints_.end()){
			int n = (int)ceil((*pleft)->getPos()->x * maxValue);

			while (n < ceil((*pright)->getPos()->x * maxValue)){
				vec4 lrgba = *(*pleft)->getRgba();
				vec4 rrgba = *(*pright)->getRgba();
				float lx = (*pleft)->getPos()->x * maxValue;
				float rx = (*pright)->getPos()->x * maxValue;
				dataArray[n] = (n - lx)/(rx - lx)*(rrgba - lrgba) + lrgba;
				n++;
			}
			pleft++;
			pright++;
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
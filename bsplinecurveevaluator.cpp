#include "bsplinecurveevaluator.h"
#include <cassert>

BSplineCurveEvaluator::BSplineCurveEvaluator(){
    _numSamples = 10;}

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										 std::vector<Point>& ptvEvaluatedCurvePts, 
										 const float& fAniLength, 
										 const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
    std::vector<double> samplePoints(_numSamples + 1,_numSamples);
    ufGenSample uf;
    std::transform(samplePoints.begin(),samplePoints.end(),samplePoints.begin(),uf);

	if (bWrap) {
		
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.
		Mat4d basisMat(1.0f/6.0f,-1.0f/2.0f,1.0f/2.0f,-1.0f/6.0f,2.0f/3.0f,0.0f,-1.0f,1.0f/2.0f,1.0f/6.0f,1.0f/2.0f,1.0f/2.0f,-1.0f/2.0f,0.0f,0.0f,0.0f,1.0f/6.0f);
		
		std::vector<Point> knotVector;
		knotVector.push_back(ptvCtrlPts[0]);
		knotVector.push_back(ptvCtrlPts[0]);
		for(std::vector<Point>::const_iterator itC = ptvCtrlPts.begin(); itC!=(ptvCtrlPts.end()-3);++itC){
			knotVector.push_back(*itC);
			knotVector.push_back(*(1+itC));
			knotVector.push_back(*(2+itC));
			knotVector.push_back(*(3+itC));
		}
		knotVector.push_back(*(ptvCtrlPts.end() - 1));
		knotVector.push_back(*(ptvCtrlPts.end() - 1));

		for(int i = 0;i<knotVector.size()/4;++i){
			Point P0 = knotVector[4*i + 1];
			Point P1 = knotVector[4*i + 2];
			Point P2 = knotVector[4*i + 3];
			Point P3 = knotVector[4*i + 4];
			for(std::vector<double>::iterator it = samplePoints.begin();it!=samplePoints.end();++it){
				Vec4d sampleMultVec(1,*it,(*it) * (*it), (*it) * (*it) * (*it));
				Vec4d result = basisMat * sampleMultVec;
				ptvEvaluatedCurvePts.push_back((result[0]*P0) + (result[1]*P1) + (result[2]*P2) + (result[3]*P3));
			}
		}
	}
}

void BSplineCurveEvaluator::setNumSamples(size_t numSamples){
    this->_numSamples = numSamples;}
const size_t BSplineCurveEvaluator::getNumSamples(){
    return _numSamples;}

#include "beziercurveevaluator.h"
#include <cassert>
#include <iostream>


BezierCurveEvaluator::BezierCurveEvaluator(){
    _numSamples = 100;}

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										 std::vector<Point>& ptvEvaluatedCurvePts, 
										 const float& fAniLength, 
										 const bool& bWrap) const
{
    ptvEvaluatedCurvePts.clear();
    if(ptvCtrlPts.size()<4)
    	return;
    std::vector<double> samplePoints(_numSamples + 1,_numSamples);
    ufGenSample uf;
    std::transform(samplePoints.begin(),samplePoints.end(),samplePoints.begin(),uf);
	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

        //this is a bell
	}
	else {
        int numIter = ((ptvCtrlPts.size() - 4)/3) + 1;
        for(int i =0;i<numIter;++i){
        	std::queue<Point> beginQueue(std::deque<Point>(ptvCtrlPts.begin()+(3*i), ptvCtrlPts.begin()+(3*i + 4)));
	        size_t numControlPoints = beginQueue.size();
	        for(std::vector<double>::iterator it = samplePoints.begin();it!=samplePoints.end();++it){
	            std::queue<Point> tempQueue = beginQueue;
	            size_t currControlPoints = numControlPoints;
	            size_t controlPointsCounter = numControlPoints;
	            while(currControlPoints>2){
	                Point first = tempQueue.front();
	                tempQueue.pop();
	                --controlPointsCounter;
	                if(controlPointsCounter == 0){
	                    --currControlPoints;
	                    controlPointsCounter = currControlPoints;
	                    continue;}
	                Point second = tempQueue.front();
	                tempQueue.push(((1.0f - *it)*first)+((*it)*second));}
	            Point first = tempQueue.front();
	            tempQueue.pop();
	            Point second = tempQueue.front();
	            tempQueue.pop();
	            ptvEvaluatedCurvePts.push_back(((1.0f - *it)*first)+((*it)*second));}}}}

void BezierCurveEvaluator::setNumSamples(size_t numSamples){
    this->_numSamples = numSamples;}
const size_t BezierCurveEvaluator::getNumSamples(){
    return _numSamples;}

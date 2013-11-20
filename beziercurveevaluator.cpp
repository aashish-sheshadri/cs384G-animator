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
            ptvEvaluatedCurvePts.push_back(((1.0f - *it)*first)+((*it)*second));}}
    if(!bWrap){
        ptvEvaluatedCurvePts.push_back(Point(0.0f,ptvCtrlPts[0].y));
        ptvEvaluatedCurvePts.push_back(Point(fAniLength,((*(ptvCtrlPts.begin()+(3*(numIter-1) + 3))).y)));
    } else {
        int numExtPoints = (ptvCtrlPts.size() - 4)%3;
        if(numExtPoints == 2){
            std::queue<Point> beginQueue(std::deque<Point>(ptvCtrlPts.end()-3, ptvCtrlPts.end()));
            beginQueue.push(Point(ptvCtrlPts[0].x+fAniLength,ptvCtrlPts[0].y));
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
                Point evalPt = ((1.0f - *it)*first)+((*it)*second);
                if(evalPt.x>fAniLength)
                    evalPt.x-=fAniLength;
                ptvEvaluatedCurvePts.push_back(evalPt);}
        } else{
            double slope = (((*(ptvCtrlPts.begin()+(3*(numIter-1) + 3))).y) - ptvCtrlPts[0].y)/(((*(ptvCtrlPts.begin()+(3*(numIter-1) + 3))).x) - ptvCtrlPts[0].x);
            double c = ptvCtrlPts[0].y - slope*ptvCtrlPts[0].x;
            double endY = slope*fAniLength + c;
            double beginY = c;
            ptvEvaluatedCurvePts.push_back(Point(0.0f,beginY));
            ptvEvaluatedCurvePts.push_back(Point(fAniLength,endY));
        }
    }
    }

void BezierCurveEvaluator::setNumSamples(size_t numSamples){
    this->_numSamples = numSamples;}
const size_t BezierCurveEvaluator::getNumSamples(){
    return _numSamples;}

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

    // if wrapping is off, make the first and last segments of
    // the curve horizontal.
    Mat4d basisMat(1.0f/6.0f,-1.0f/2.0f,1.0f/2.0f,-1.0f/6.0f,2.0f/3.0f,0.0f,-1.0f,1.0f/2.0f,1.0f/6.0f,1.0f/2.0f,1.0f/2.0f,-1.0f/2.0f,0.0f,0.0f,0.0f,1.0f/6.0f);

    std::vector<Point> knotVector;
    if(~bWrap){
        knotVector.push_back(ptvCtrlPts[0]);
        knotVector.push_back(ptvCtrlPts[0]);}
    for(std::vector<Point>::const_iterator itC = ptvCtrlPts.begin(); itC!=ptvCtrlPts.end();++itC)
        knotVector.push_back(*itC);
    if(bWrap){
        knotVector.push_back(*(ptvCtrlPts.end() - 1));
        knotVector.push_back(Point(fAniLength + ptvCtrlPts[0].x, ptvCtrlPts[0].y));
        knotVector.push_back(Point(fAniLength + ptvCtrlPts[0].x, ptvCtrlPts[0].y));}

    if(~bWrap){
        knotVector.push_back(*(ptvCtrlPts.end() - 1));
        knotVector.push_back(*(ptvCtrlPts.end() - 1));}

    for(std::vector<Point>::iterator it = knotVector.begin();it!=(knotVector.end()-3);++it){
        Point P0 = *it;
        Point P1 = *(1 + it);
        Point P2 = *(2 + it);
        Point P3 = *(3 + it);
        for(std::vector<double>::iterator it = samplePoints.begin();it!=samplePoints.end();++it){
            Vec4d sampleMultVec(1,*it,(*it) * (*it), (*it) * (*it) * (*it));
            Vec4d result = basisMat * sampleMultVec;
            Point evalPt = (result[0]*P0) + (result[1]*P1) + (result[2]*P2) + (result[3]*P3);
            if(bWrap){
                if(evalPt.x>fAniLength){
                    evalPt.x-=fAniLength;
                    if(evalPt.x>ptvCtrlPts[0].x)
                        continue;}}
            ptvEvaluatedCurvePts.push_back(evalPt);
        }
    }
    if(!bWrap){
        ptvEvaluatedCurvePts.push_back(Point(0.0f,ptvCtrlPts[0].y));
        ptvEvaluatedCurvePts.push_back(Point(fAniLength,(*(ptvCtrlPts.end() -1)).y));
    }
}

void BSplineCurveEvaluator::setNumSamples(size_t numSamples){
    this->_numSamples = numSamples;}
const size_t BSplineCurveEvaluator::getNumSamples(){
    return _numSamples;}

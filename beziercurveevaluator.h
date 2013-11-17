#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "curveevaluator.h"
#include <queue>
#include <deque>
//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
	size_t _numSamples;
public:
    BezierCurveEvaluator();
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const;
	void setNumSamples(size_t);
	const size_t getNumSamples(void);

};

#endif

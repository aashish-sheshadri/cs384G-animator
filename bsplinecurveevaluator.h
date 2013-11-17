#ifndef INCLUDED_BSPLINE_CURVE_EVALUATOR_H
#define INCLUDED_BSPLINE_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "curveevaluator.h"
#include "mat.h"
#include "vec.h"

//using namespace std;

class BSplineCurveEvaluator : public CurveEvaluator
{
	size_t _numSamples;
public:
    BSplineCurveEvaluator();
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const;
	void setNumSamples(size_t);
	const size_t getNumSamples(void);
};

#endif

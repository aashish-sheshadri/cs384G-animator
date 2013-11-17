#ifndef INCLUDED_CURVE_EVALUATOR_H
#define INCLUDED_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)

#include "curve.h"

//using namespace std;
struct ufGenSample{
    size_t _u;
    ufGenSample(size_t u = -1):_u(u){}
    double operator () (double val){
        ++_u;
        return((double)_u/val);}};
class CurveEvaluator
{
public:
	virtual ~CurveEvaluator(void);
	virtual void evaluateCurve(const std::vector<Point>& control_points, 
							   std::vector<Point>& evaluated_curve_points, 
							   const float& animation_length, 
							   const bool& wrap_control_points) const = 0;
	static float s_fFlatnessEpsilon;
	static int s_iSegCount;
};


#endif

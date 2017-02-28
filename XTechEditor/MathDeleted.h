#ifndef _MATH_DELETED_h
#define _MATH_DELETED_h


#include "XMathBase.h"
#include "Vectors.h"
#include <iostream>

class  Plane_ {

	public:
		vec3 normal;
		float dist;

		Plane_() : normal(0.0f, 0.0f, 1.0f), dist(0.0f) {}
		Plane_(const vec3 &n, float d) : normal(n), dist(d) {}
		Plane_(const vec3 &, const vec3 &);

		vec3 getOrigin();
		void setOrigin(const vec3 &);

		void normalize();
		friend Plane_ normalize(const Plane_ &);
		bool Plane_::trace(const Line &l, TracePoint &i, bool fixSrc, bool fixDst) const;
	};



    

	#endif
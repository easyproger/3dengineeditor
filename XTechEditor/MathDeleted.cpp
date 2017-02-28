#include "MathDeleted.h"


Plane_::Plane_(const vec3 &n, const vec3 &o) {
		normal = n;
		setOrigin(o);
	}

	//--------------------------------------------------------------------------

	void Plane_::setOrigin(const vec3 &o) {
		dist = -dot_(normal, o);
	}

	//--------------------------------------------------------------------------

	void Plane_::normalize() {
		float len = normal.length();

		if (abs(len) > EPSILON ) {
			normal /= len;
			dist /= len;
		}
	}

	//--------------------------------------------------------------------------

	Plane_ normalize(const Plane_ &p) {
		float len = p.normal.length();

		if (abs(len) > EPSILON ) {
			float ilen = 1.0f / len;
			return Plane_(p.normal * ilen, p.dist * ilen);
		}
		return p;
	}

	//--------------------------------------------------------------------------

	bool Plane_::trace(const Line &l, TracePoint &i, bool fixSrc, bool fixDst) const {

		float cosang = 0;
		float d = 0;
		float lamda = 0;

		vec3 dir = l.getDirection();
		cosang = dot_(dir, normal);

		if (cosang > -EPSILON && cosang < EPSILON) return false;

		d = dot_(l.src, normal);

		lamda = (-dist - d) / cosang;
		if (lamda > 100.0) lamda = 100.0;
		if (lamda < -100.0) lamda = -100.0;
		
		if ((fixSrc && lamda < 0) || (fixDst && lamda > 1)) return false;

		i.point = l.src + dir * lamda;
		i.normal = normal;

		return true;
	}

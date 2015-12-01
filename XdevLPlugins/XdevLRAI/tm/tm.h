/*
	Copyright (c) 2003-2005 Cengiz Terzibas

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Cengiz Terzibas         cengiz@terzibas.de
*/


#ifndef TINY_MATH_H
#define TINY_MATH_H

#include <iostream>
#include <cmath>
#include <cfloat>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __SSE2__
extern "C" {
#include <emmintrin.h>
#include <xmmintrin.h>
}
#endif

#ifdef __SSE3__
extern "C" {
#include <pmmintrin.h>
#include <immintrin.h>
}
#endif

#ifdef __SSE4__
extern "C" {
#include <smmintrin.h>
}
#endif

namespace tmath {

	//
	// Forward declrarations.
	//
	template<typename T, int N, int M> class matrix;
	template<typename T> class matrix<T,3,3>;
	template<typename T> class matrix<T,4,4>;
	template<typename T, int N> class vectorn;
	template<typename T> class vectorn<T, 3>;
	template<typename T> class vectorn<T, 4>;
	template <typename T> class quaternion;

	//
	// Some usefull typedefs.
	//
	typedef vectorn<float, 2> 		vec2;
	typedef vectorn<float, 3> 		vec3;
	typedef vectorn<float, 4> 		vec4;
	typedef matrix<float, 2, 2> 	mat2;
	typedef matrix<float, 3, 3> 	mat3;
	typedef matrix<float, 4, 4> 	mat4;
	typedef quaternion<float>			quat;

	/// Converts degres to radian
	template<typename T>
	inline const T d2r(const T a) {
		return (a * static_cast<T>(M_PI))/static_cast<T>(180.0);
	}

	/// Converts radian to degres
	template<typename T>
	inline const T r2d(const T a) {
		return (a * static_cast<T>(180.0f))/static_cast<T>(M_PI);
	}

	/// Returns the sign of a value.
	inline float sign(float x) {
		return (x >= 0.0f) ? +1.0f : -1.0f;
	}

	//
	// quaternion templates.
	//
	
	template<typename T> T norm(const quaternion<T>& q);
	template<typename T> T length(const quaternion<T>& q);
	template<typename T> quaternion<T>& normalize(quaternion<T>& q);
	template<typename T> quaternion<T> cross(const quaternion<T>& q1, const quaternion<T>& q2);
	template<typename T> quaternion<T> conjugate(const quaternion<T>& q);
	template<typename T> quaternion<T> inverse(const quaternion<T>& q);
	template<typename T> matrix<T,3,3>& convert(const quaternion<T>& q, matrix<T,3,3>& m);
	template<typename T> matrix<T,4,4>& convert(const quaternion<T>& q, matrix<T,4,4>& m);
	template<typename T> void rotate_x(const T angle, quaternion<T>& q);
	template<typename T> void rotate_y(const T angle, quaternion<T>& q);
	template<typename T> void rotate_z(const T angle, quaternion<T>& q);
	template<typename T> vectorn<T,3> rotate(const quaternion<T>& q, const vectorn<T,3>& axis);
	template<typename T> void rotate(const T angle, const vectorn<T,3> &axis, quaternion<T>& q);
	template<typename T> void euler_to_quaternion(T yaw, T pitch, T roll, quaternion<T>& q);
	template<typename T> void euler_to_quaternion(T yaw, T pitch, quaternion<T>& q);
	template<typename T> void quaternion_to_euler(const quaternion<T>& q, T& anglex, T& angley, T& anglez);
	template<typename T> quaternion<T>& slerp(quaternion<T>& q1, quaternion<T>& q2, T t, quaternion<T>& q);
	template<typename T> void identity(quaternion<T>& q);
	template<typename T> std::ifstream& operator>>(std::ifstream& in, quaternion<T> &q);
	
	//
	// vector<T,3> templates.
	// TODO Needs to be fixed.
	
	template<typename T> vectorn<T,3>& normalize(vectorn<T,3>& v);
	template<typename T> T length(const vectorn<T,3>& v);
	template<typename T> vectorn<T,3> cross(const vectorn<T,3>& v1,const vectorn<T,3>& v2);
	template<typename T> void cross(const vectorn<T,3>& v1,const vectorn<T,3>& v2, vectorn<T,3>& rv);
	template<typename T> vectorn<T,4>& convert(const vectorn<T,3>& v1, vectorn<T,4>& v2);
	template<typename T> std::ifstream& operator >> (std::ifstream& in, vectorn<T,3> &v);

	//
	// vector<T,4> templates.
	//
	
	template<typename T> T norm(const vectorn<T,4>& v);
	template<typename T> vectorn<T,4>& normalize(vectorn<T,4>& v);
	template<typename T> T length(const vectorn<T,4>& v);
	template<typename T> vectorn<T,3>& convert(const vectorn<T,4>& v1, vectorn<T,3>& v2);
	template<typename T> std::ifstream& operator >> (std::ifstream& in, vectorn<T,4> &v);

	//
	// matrix<T,3,3> templates.
	// TODO Needs to be fixed.

	template<typename T> const matrix<T,3,3> transpose(const matrix<T,3,3>& m);
	template<typename T> T determinant(const matrix<T,3,3>& m);
	template<typename T> matrix<T,3,3>&  convert(const matrix<T,4,4>& m1, matrix<T,3,3>& m2);

	//
	// matrix<T,4,4> templates.
	//
	
	template<typename T> const matrix<T,4,4> transpose(const matrix<T,4,4>& m);
	template<typename T> T determinant(const matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& translate(const T x, const T y, const T z, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& translate(const vectorn<T,3>& v, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& scale(const T sx, const T sy, const T sz, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& scale(const vectorn<T,3>& v, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& rotate_x(const T& angle, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& rotate_y(const T& angle, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& rotate_z(const T& angle, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& rotate(const T& angle, const vectorn<T,3>& axis, matrix<T,4,4>& mat);
	template<typename T> matrix<T,4,4>& frustum(T left, T right, T bottom, T top, T near, T far, matrix<T,4,4>& mat);
	template<typename T> matrix<T,4,4>& ortho(T left, T right, T bottom, T top, T near, T far, matrix<T,4,4>& mat);
	template<typename T> matrix<T,4,4>& perspective(T fov, T aspect, T near, T far, matrix<T,4,4>& mat);
	template<typename T> matrix<T,4,4>& look_at(vectorn<T,3>& position, vectorn<T,3>& target, vectorn<T,3>& up, matrix<T,4,4>& m);
	template<typename T> matrix<T,4,4>& zero(matrix<T,4,4>& mat);
	template<typename T> matrix<T,4,4>& identity(matrix<T,4,4>& mat);
	template<typename T> quaternion<T>& convert(const matrix<T,4,4>& mat, quaternion<T>& quat);
	template<typename T> matrix<T,4,4> inverse(const matrix<T,4,4>& m);
	template<typename T> std::ifstream& operator>>(std::ifstream& in, matrix<T,4,4>& m);
}

#include <tm/matrix.h>
#include <tm/matrix3.h>
#include <tm/matrix4.h>
#include <tm/quaternion.h>
#include <tm/vector3.h>
#include <tm/vector4.h>
#include <tm/vectorn.h>

#endif

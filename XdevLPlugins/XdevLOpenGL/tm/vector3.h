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

#ifndef VECTOR3_H
#define VECTOR3_H

namespace tmath {

	/**
		@author 		Cengiz Terzibas
	*/

	template<typename T>
	class vectorn<T, 3> {
		public:
			typedef T value_type;
			union {
				T x, r, s;
			};
			union {
				T y, g, t;
			};
			union {
				T z, b, p;
			};
			// constructors
			vectorn<T,3>() :	x(static_cast<T>(0.0)),
				y(static_cast<T>(0.0)),
				z(static_cast<T>(0.0)) {
			}
			vectorn<T,3>(T vx, T vy, T vz) : x(vx), y(vy), z(vz) {
			}
			vectorn<T,3>(const T* v) : x(v[0]), y(v[1]), z(v[2]) {
			}
			vectorn<T,3>(const vectorn<T,3>& v) : x(v.x), y(v.y), z(v.z) {
			}
			// assignment operations
			inline const vectorn<T,3>& operator+=(const vectorn<T,3>& v) {
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}
			inline const vectorn<T,3>& operator-=(const vectorn<T,3>& v) {
				x -= v.x;
				y -= v.y;
				z -= v.z;
				return *this;
			}
			inline const vectorn<T,3>& operator*=(const T& num) {
				x *= num;
				y *= num;
				z *= num;
				return *this;
			}
			inline const vectorn<T,3>& operator/=(const T& num) {
				const T r = static_cast<T>(1.0)/num;
				x *= r;
				y *= r;
				z *=r;
				return *this;
			}
			// stream operations
			inline friend std::ostream& operator<<(std::ostream& out, const vectorn<T,3>& v)	{
				out << "[" << v.x << "," << v.y << "," << v.z << "]";
				return out;
			}
			// comparison operations
			inline const bool operator == ( const vectorn<T,3>& v ) const {
				return (v.x==x && v.y==y && v.z==z);
			}
			inline const bool operator != ( const vectorn<T,3>& v ) const {
				return !(v == *this);
			}
			// unary operations
			inline const vectorn<T,3> operator - () const {
				return vectorn<T,3>( -x, -y, -z);
			}
			// binary operations
			inline friend const vectorn<T,3> operator+(const vectorn<T,3>& v1, const vectorn<T,3>& v2) {
				return vectorn<T,3>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
			}
			inline friend const vectorn<T,3> operator-(const vectorn<T,3>& v1, const vectorn<T,3>& v2) {
				return vectorn<T,3>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
			}
			inline friend const T	operator*(const vectorn<T,3>& v1,const vectorn<T,3>& v2) {
				return T(v1.x * v2.x + v1.y *v2.y + v1.z * v2.z);
			}
			inline const vectorn<T,3> operator*(const T& num) const {
				return vectorn<T,3>(x * num, y * num, z * num);
			}
			friend inline const vectorn<T,3> operator * ( const T& s, const vectorn<T,3>& v ) {
				return v * s;
			}
			inline const vectorn<T,3> operator/(const T& num) const {
				const T r = static_cast<T>(1.0)/num;
				return vectorn<T,3>(x * r, y * r, z * r);
			}
			size_t size() const {
				return 3;
			}
			// cast operations
			operator T*() {
				return &x;
			}
			operator const T*() const {
				return &x;
			}
	};

	/// Calculate the normal of a vectorn<T,3>
	template<typename T>
	inline T norm(const vectorn<T,3>& v) {
		return static_cast<T>(v.x*v.x + v.y*v.y + v.z*v.z);
	}
	
/// Normalize a vectorn<T,3>
	template<typename T>
	vectorn<T,3>& normalize(vectorn<T,3>& v) {
		T value = static_cast<T>(1.0)/sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
		v.x *= value;
		v.y *= value;
		v.z *= value;
		return v;
	}	


	/// Calculate the length of a vectorn<T,3>
	template<typename T>
	T length(const vectorn<T,3>& v) {
		return static_cast<T>(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
	}

	/// Calculates the cross product of two vectorn<T,3>
	template<typename T>
	vectorn<T,3> cross(const vectorn<T,3>& v1,const vectorn<T,3>& v2) {
		return tmath::vectorn<T,3>((v1.y * v2.z) - (v1.z * v2.y),
		                           (v1.z * v2.x) - (v1.x * v2.z),
		                           (v1.x * v2.y) - (v1.y * v2.x));

	}

	/// Calculates the cross product of two vectorn<T,3>
	template<typename T>
	void cross(const vectorn<T,3>& v1,const vectorn<T,3>& v2, vectorn<T,3>& rv) {
		rv.x = (v1.y * v2.z) - (v1.z * v2.y);
		rv.y = (v1.z * v2.x) - (v1.x * v2.z);
		rv.z = (v1.x * v2.y) - (v1.y * v2.x);

	}
	
	template<typename T>
	std::ifstream& operator >> (std::ifstream& in, vectorn<T,3> &v) {
		in >> v.x >>  v.y >> v.z;
		return in;
	}
	
	/// Convert vectorn<T,3> to vectorn
	template<typename T>
	vectorn<T,4>& convert(const vectorn<T,3>& v1, vectorn<T,4>& v2) {
		v2.x = v1.x;
		v2.y = v1.y;
		v2.z = v1.z;
		v2.w = 0.0f;
		return v2;
	}
}

#endif

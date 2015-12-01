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

#ifndef QUATERNION_H
#define QUATERNION_H

namespace tmath {

	/**
		@class 			quaternion
		@brief 			Class that represents a quaternion
		@author 		Cengiz Terzibas
	*/
	template <typename T>
	class quaternion {
	public:
		
			T x, y, z, w;
			
			// constructors
			quaternion<T>() : x(static_cast<T>(0.0)),
				y(static_cast<T>(0.0)),
				z(static_cast<T>(0.0)),
				w(static_cast<T>(1.0)) {
			}
			
			quaternion<T>(T qx, T qy, T qz, T qw) : x(qx), y(qy), z(qz), w(qw) {
			}
			
			quaternion<T>(const T* q) : x(q[0]), y(q[1]), z(q[2]), w(q[3]) {
			}
			
			quaternion<T>(const quaternion<T>& q) : x(q.x), y(q.y), z(q.z), w(q.w) {
			}
			
			// assignment operations
			inline const quaternion<T> operator+=(const quaternion<T> &q) {
				w += q.w;
				x += q.x;
				y += q.y;
				z += q.z;
				return *this;
			}
			
			inline const quaternion<T> operator-=(const quaternion<T> &q) {
				w -= q.w;
				x -= q.x;
				y -= q.y;
				z -= q.z;
				return *this;
			}
			
			inline quaternion<T> operator*=(const T &qs) {
				x *= qs;
				y *= qs;
				z *= qs;
				w *= qs;
				return *this;
			}
			
			// stream operations
			inline friend std::ostream& operator<<(std::ostream& out, const quaternion<T>& q) {
				out << "[" << q.x << "," << q.y << "," << q.z << "," << q.w << "]";
				return out;
			}
			
			// comparison operations
			inline const bool operator == (const quaternion<T>& q) const {
				return (q.x==x && q.y==y && q.z==z && q.w==w);
			}
			
			inline const bool operator != (const quaternion<T>& q) const {
				return !(q == *this);
			}
			
			// unary operations
			inline const quaternion<T> operator - () const {
				return quaternion<T>(-x, -y, -z, -w);
			}
			
			// binary operations
			inline friend const quaternion<T> operator+(const quaternion<T> &q1, const quaternion<T> &q2) {
				return quaternion<T>(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
			}
			
			inline friend const quaternion<T> operator-(const quaternion<T> &q1, const quaternion<T> &q2) {
				return quaternion<T>(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
			}
			
			inline quaternion<T> operator*(const T &qs) const {
				return quaternion<T>(x * qs, y * qs, z * qs,  w * qs);
			}
			
			friend inline quaternion<T> operator*(const T& pS, const quaternion<T>& q) {
				return quaternion<T>(q.x * pS, q.y * pS, q.z * pS, q.w * pS);
			}
			
			inline friend const quaternion<T> operator * (const quaternion<T>& q1, const quaternion<T>& q2) {
				return quaternion<T>(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
				                     q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
				                     q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
				                     q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
			}

			inline friend const vectorn<T, 3> operator * (const quaternion<T>& q1, const vectorn<T, 3>& v) {

				vectorn<T, 3> uv, uuv;
				vectorn<T, 3> qvec(q1.x, q1.y, q1.z);
				uv 	= cross(qvec, v);
				uuv = cross(qvec, uv);
				uv *= (2.0f * q1.w);
				uuv *= 2.0f;
				return (v + uv + uuv);
			}

			inline friend const vectorn<T, 3> operator * (const vectorn<T, 3>& v, const quaternion<T>& q1) {

				vectorn<T, 3> uv, uuv;
				vectorn<T, 3> qvec(q1.x, q1.y, q1.z);
				uv 	= cross(qvec, v);
				uuv = cross(qvec, uv);
				uv 	*= (2.0f * q1.w);
				uuv *= 2.0f;
				return (v + uv + uuv);
			}

			// cast operations
			operator T*() {
				return &x;
			}
			
			operator const T*() const {
				return &x;
			}
			
	};
	
	/// Calculates the norm of a quaternion
	template<typename T>
	T norm(const quaternion<T>& q) {
		return (q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	}

	/// Calculates the length of a quaternion
	template<typename T>
	T length(const quaternion<T>& q) {
		return static_cast<T>(sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z));
	}

	/// Normalize the quaternion
	template<typename T>
	quaternion<T>& normalize(quaternion<T>& q) {
		T normFactor = static_cast<T>(1.0) / length(q);
		q.w *= normFactor;
		q.x *= normFactor;
		q.y *= normFactor;
		q.z *= normFactor;
		return q;
	}	
	
	template<typename T>
	quaternion<T> cross( const quaternion<T>& q1, const quaternion<T>& q2) {
		return quaternion<T>(q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		                            q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		                            q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		                            q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x);
	}	
	
	/// conjugate the quaternion<T>
	template<typename T>
	quaternion<T> conjugate(const quaternion<T>& q) {
		tmath::quaternion<T> qres(q);
		qres.x	*= static_cast<T>(-1.0);
		qres.y 	*= static_cast<T>(-1.0);
		qres.z	*= static_cast<T>(-1.0);
		return qres;
	}

	/// invert the quaternion<T>
	template<typename T>
	quaternion<T> inverse(const quaternion<T>& q) {
		quaternion<T> qres(q);
		T s = length(qres);
		if( s > 0.0) {
			s = 1.0f/s;
			qres*=s;
			return conjugate(qres);
		}
		return quaternion<T>(0.0, 0.0, 0.0, 0.0);
	}	
	
	/// Rotate a vector using a quaternion.
	template<typename T>
	vectorn<T,3> rotate(const quaternion<T>& q, const vectorn<T,3>& axis) {
		quaternion<T> tmp(axis);
		tmp = q * tmp * conjugate(q);
		vectorn<T,3> tmp2(tmp.x, tmp.y, tmp.z);
		return tmp2;
	}


	/// Converts a quaternion into a matrix<T,3,3>
	template<typename T>
	matrix<T,3,3>& convert(const quaternion<T>& q, matrix<T,3,3>& m ) {
		m.xx = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.y * q.y + q.z * q.z );
		m.xy = static_cast<T>(2.0) * (q.x * q.y - q.z * q.w);
		m.xz = static_cast<T>(2.0) * (q.x * q.z + q.y * q.w);

		m.yx = static_cast<T>(2.0) * ( q.x * q.y + q.z * q.w );
		m.yy = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.z * q.z );
		m.yz = static_cast<T>(2.0) * (q.z * q.y - q.x * q.w );

		m.zx = static_cast<T>(2.0) * ( q.x * q.z - q.y * q.w );
		m.zy = static_cast<T>(2.0) * ( q.y * q.z + q.x * q.w );
		m.zz = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.y * q.y );
		return m;
	}

	/// Converts a quaternion into a matrix4x4<T>
	template<typename T>
	matrix<T,4,4>& convert(const quaternion<T>& q, matrix<T,4,4>& m) {
		m[0]= static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.y * q.y + q.z * q.z );
		m[4] = static_cast<T>(2.0) * (q.x * q.y - q.z * q.w);
		m[8] = static_cast<T>(2.0) * (q.x * q.z + q.y * q.w);
		m[12] = static_cast<T>(0.0);

		m[1] = static_cast<T>(2.0) * ( q.x * q.y + q.z * q.w );
		m[5] = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.z * q.z );
		m[9] = static_cast<T>(2.0) * (q.z * q.y - q.x * q.w );
		m[13] = static_cast<T>(0.0);

		m[2] = static_cast<T>(2.0) * ( q.x * q.z - q.y * q.w );
		m[6] = static_cast<T>(2.0) * ( q.y * q.z + q.x * q.w );
		m[10] = static_cast<T>(1.0) - static_cast<T>(2.0) * ( q.x * q.x + q.y * q.y );
		m[14] = static_cast<T>(0.0);

		m[3] = static_cast<T>(0.0);
		m[7] = static_cast<T>(0.0);
		m[11] = static_cast<T>(0.0);
		m[15] = static_cast<T>(1.0);
		return m;
	}
	
	/// Calculate a quaternion that represents a rotation around the x axis
	template<typename T>
	void rotate_x(const T angle, quaternion<T>& q) {
		T rad = d2r(angle*static_cast<T>(0.5));
		q.w = cos(rad);
		q.x = sin(rad);
		q.y = static_cast<T>(0.0);
		q.z = static_cast<T>(0.0);
	}

	/// Calculate a quaternion that represents a rotation around the y axis
	template<typename T>
	void rotate_y(const T angle, quaternion<T>& q) {
		T rad = d2r(angle*static_cast<T>(0.5));
		q.w = cos(rad);
		q.x = static_cast<T>(0.0);
		q.y = sin(rad);
		q.z = static_cast<T>(0.0);
	}

	/// Calculate a quaternion that represents a rotation around the z axis
	template<typename T>
	void rotate_z(const T angle, quaternion<T>& q) {
		T rad = d2r(angle*static_cast<T>(0.5));
		q.w = cos(rad);
		q.x = static_cast<T>(0.0);
		q.y = static_cast<T>(0.0);
		q.z = sin(rad);
	}

	template<typename T>
	void rotate(const T angle, const vectorn<T,3> &axis, quaternion<T>& q) {
		T rad = d2r(angle*static_cast<T>(0.5));
		q.w = cos(rad);
		q.x = axis.x * sin(rad);
		q.y = axis.y * sin(rad);
		q.z = axis.z * sin(rad);
	}

	/// Converts euler angles into a quaternion orienation
	template<typename T>
	void euler_to_quaternion(T yaw, T pitch, T roll, quaternion<T>& q) {
		T cosYaw		= cos(yaw * 0.5);
		T sinYaw		= sin(yaw * 0.5);
		T cosPitch	= cos(pitch * 0.5);
		T sinPitch	= sin(pitch * 0.5);
		T cosRoll		= cos(roll * 0.5);
		T sinRoll		= sin(roll * 0.5);
		q.x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		q.y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		q.z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		q.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	}

	/// Converts euler angles into a quaternion orienation
	template<typename T>
	void euler_to_quaternion(T yaw, T pitch, quaternion<T>& q) {
		T cosYaw		= cos(yaw * 0.5);
		T sinYaw		= sin(yaw * 0.5);
		T cosPitch	= cos(pitch * 0.5);
		T sinPitch	= sin(pitch * 0.5);
		q.x = sinPitch * cosYaw;
		q.y = cosPitch * sinYaw;
		q.z = -sinPitch * sinYaw;
		q.w = cosPitch * cosYaw;
	}

	/// Converts a quaternion orientation into euler angles
	template<typename T>
	void quaternion_to_euler(const quaternion<T>& q, T& anglex, T& angley, T& anglez) {
		anglex 	= atan(2*(q.y*q.z + q.w*q.x)/ (q.w*q.w - q.x*q.w - q.y*q.y + q.z*q.z));
		angley  = asin(-2*(q.x*q.z - q.w*q.y));
		anglez  = atan(2*(q.x*q.y + q.w*q.z)/ (q.w*q.w+ q.x*q.x - q.y*q.y - q.z*q.z));
	}
	
	/// Create a quaternion that represents a spherical interpolation
	template<typename T>
	quaternion<T>& slerp(quaternion<T>& q1, quaternion<T>& q2, T t, quaternion<T>& q) {
		
		if(q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w)
			return q1;

		T result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

		if(result < 0.0f) {
			q2 = quaternion<T>(-q2.x, -q2.y, -q2.z, -q2.w);
			result = -result;
		}

		T scale0 = 1 - t, scale1 = t;
		if(1 - result > 0.1f) {
			T theta = (T)acos(result);
			T sinTheta = (T)sin(theta);

			scale0 = (T)sin((1 - t) * theta) / sinTheta;
			scale1 = (T)sin((t * theta)) / sinTheta;
		}

		q.x = (scale0 * q1.x) + (scale1 * q2.x);
		q.y = (scale0 * q1.y) + (scale1 * q2.y);
		q.z = (scale0 * q1.z) + (scale1 * q2.z);
		q.w = (scale0 * q1.w) + (scale1 * q2.w);

		return q;
	}	
	
		/// Make an identity quaternion<T>
	template<typename T>
	void identity(quaternion<T>& q) {
		q.x = 0.0;
		q.y = 0.0;
		q.z = 0.0;
		q.w = 1.0;
	}
	
	template<typename T>
	std::ifstream& operator>>(std::ifstream& in, quaternion<T> &q) {
		in >> q.x >>  q.y >> q.z >> q.w;
		return in;
	}
	
}

#endif

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

#ifndef MATRIX3_H
#define MATRIX3_H

namespace tmath {

	/**
		@author 		Cengiz Terzibas
	*/

	template<typename T>
	class matrix<T,3,3> {
		public:
			typedef T value_type;

			T xx, yx, zx;
			T xy, yy, zy;
			T xz, yz, zz;

			// constructors
			matrix<T,3,3>()	:	xx(static_cast<T>(0.0)),
				xy(static_cast<T>(0.0)),
				xz(static_cast<T>(0.0)),
				yx(static_cast<T>(0.0)),
				yy(static_cast<T>(0.0)),
				yz(static_cast<T>(0.0)),
				zx(static_cast<T>(0.0)),
				zy(static_cast<T>(0.0)),
				zz(static_cast<T>(0.0)) {
			}
			matrix<T,3,3>(	T a0, T a1, T a2,
			                T a3,	T a4, T a5,
			                T a6, T a7, T a8)
				:	xx(a0), xy(a1), xz(a2),
				  yx(a3), yy(a4), yz(a5),
				  zx(a6), zy(a7), zz(a8) {
			}
			matrix<T,3,3>(const T* m) {
				xx = m[0];
				xy = m[1];
				xz = m[2];
				yx = m[4];
				yy = m[5];
				yz = m[6];
				zx = m[8];
				zy = m[9];
				zz = m[10];
			}
			matrix<T,3,3>(const matrix<T,3,3>& m) {
				xx = m.xx;
				xy = m.xy;
				xz = m.xz;
				yx = m.yx;
				yy = m.yy;
				yz = m.yz;
				zx = m.zx;
				zy = m.zy;
				zz = m.zz;
			}
			// assignment operations
			inline matrix<T,3,3> operator+=(const matrix<T,3,3>& m) {
				xx += m.xx;
				xy += m.xy;
				xz += m.xz;
				yx += m.yx;
				yy += m.yy;
				yz += m.yz;
				zx += m.zx;
				zy += m.zy;
				zz += m.zz;
				return *this;
			}
			inline matrix<T,3,3> operator-=(const matrix<T,3,3>& m) {
				xx -= m.xx;
				xy -= m.xy;
				xz -= m.xz;
				yx -= m.yx;
				yy -= m.yy;
				yz -= m.yz;
				zx -= m.zx;
				zy -= m.zy;
				zz -= m.zz;
				return *this;
			}
			inline matrix<T,3,3> operator*=(const T &num) {
				xx *= num;
				xy *= num;
				xz *= num;
				yx *= num;
				yy *= num;
				yz *= num;
				zx *= num;
				zy *= num;
				zz *= num;
				return *this;
			}
			inline matrix<T,3,3> operator/=(const T& num) {
				xx /= num;
				xy /= num;
				xz /= num;
				yx /= num;
				yy /= num;
				yz /= num;
				zx /= num;
				zy /= num;
				zz /= num;
				return *this;
			}
			// stream operations
			friend std::ostream& operator<<(std::ostream& out, const matrix<T,3,3>& m) {
				out << m.xx << " " << m.xy << " " << m.xz << std::endl;
				out << m.yx << " " << m.yy << " " << m.yz << std::endl;
				out << m.zx << " " << m.zy << " " << m.zz << std::endl;
				return out;
			}
			// comparison operations
			inline const bool operator == (const matrix<T,3,3>& m) const {
				return (	xx == m.xx && xy == m.xy && xz == m.xz &&
				          yx == m.yx && yy == m.yy && yz == m.yz &&
				          zx == m.zx && zy == m.zy && zz == m.zz);
			}
			inline const bool operator != ( const matrix<T,3,3>& m) const {
				return !(m == *this);
			}
			// unary operations
			inline const matrix<T,3,3> operator - () const {
				return matrix<T,3,3>(-xx, -xy, -xz,
				                     -yx, -yy, -yz,
				                     -zx, -zy, -zz);
			}
			// binary operations
			inline friend matrix<T,3,3> operator+(const matrix<T,3,3>& m1,const matrix<T,3,3>& m2) {
				return matrix<T,3,3>(m1.xx + m2.xx, m1.xy + m2.xy, m1.xz + m2.xz,
				                     m1.yx + m2.yx, m1.yy + m2.yy, m1.yz + m2.yz,
				                     m1.zx + m2.zx, m1.zy + m2.zy, m1.zz + m2.zz);
			}
			inline friend matrix<T,3,3> operator-(const matrix<T,3,3>& m1,const matrix<T,3,3>& m2) {
				return matrix<T,3,3>(m1.xx - m2.xx, m1.xy - m2.xy, m1.xz - m2.xz,
				                     m1.yx - m2.yx, m1.yy - m2.yy, m1.yz - m2.yz,
				                     m1.zx - m2.zx, m1.zy - m2.zy, m1.zz - m2.zz);
			}
			inline const matrix<T,3,3> operator*(const T& num) const {
				return matrix<T,3,3>(xx * num, xy * num, xz * num,
				                     yx * num, yy * num, yz * num,
				                     zx * num, zy * num, zz * num);
			}
			friend inline const matrix<T,3,3> operator*( const T &s, const matrix<T,3,3>& m ) {
				return m * s;
			}
			inline friend matrix<T,3,3> operator*(const matrix<T,3,3>& m1, const matrix<T,3,3>& m2 )  {
				return matrix<T,3,3>(m1.xx * m2.xx + m1.xy * m2.yx + m1.xz * m2.zx,
				                     m1.xx * m2.xy + m1.xy * m2.yy + m1.xz * m2.zy,
				                     m1.xx * m2.xz + m1.xy * m2.yz + m1.xz * m2.zz,
				                     m1.yx * m2.xx + m1.yy * m2.yx + m1.yz * m2.zx,
				                     m1.yx * m2.xy + m1.yy * m2.yy + m1.yz * m2.zy,
				                     m1.yx * m2.xz + m1.yy * m2.yz + m1.yz * m2.zz,
				                     m1.zx * m2.xx + m1.zy * m2.yx + m1.zz * m2.zx,
				                     m1.zx * m2.xy + m1.zy * m2.yy + m1.zz * m2.zy,
				                     m1.zx * m2.xz + m1.zy * m2.yz + m1.zz * m2.zz);
			}
			inline friend const vectorn<T,3> operator*(const matrix<T,3,3>& m, const vectorn<T,3>& v) {
				return vectorn<T,3>(v.x*m.xx + v.y*m.xy + v.z*m.xz,
				                    v.x*m.yx + v.y*m.yy + v.z*m.yz,
				                    v.x*m.zx + v.y*m.zy + v.z*m.zz);

			}
			inline const matrix<T,3,3> operator/(const T& num) const {
				float val = (static_cast<T>(1.0)/num);
				return matrix<T,3,3>(xx * val, xy * val, xz * val,
				                     yx * val, yy * val, yz * val,
				                     zx * val, zy * val, zz * val);
			}
			// indexing operations
			void row(unsigned int pRow, const vectorn<T,3>& pV) {
				unsigned idx = 4*pRow;
				(*this)[idx]			= pV.x;
				(*this)[idx + 1]	= pV.y;
				(*this)[idx + 2]	= pV.z;
			}
			void column(unsigned int pColumn, const vectorn<T,3>& pV) {
				(*this)[pColumn]			= pV.x;
				(*this)[pColumn + 3]	= pV.y;
				(*this)[pColumn + 6]	= pV.z;
			}
			inline const vectorn<T,3> row(int idx) {
				return vectorn<T,3>(*(&xx + 3*idx),*(&xx + 3*idx +1),*(&xx + 3*idx +2));
			}
			inline const vectorn<T,3> column(int idx) {
				return vectorn<T,3>(*(&xx + idx),*(&xx + idx+ 3),*(&xx + idx + 6));
			}
			inline void normalize_column() {
				vectorn<T,3> v0,v1,v2, u0, u1, u2;
				v0 = vectorn<T,3>(xx, yx,zx);
				v1 = vectorn<T,3>(xy, yy,zy);
				v2 = vectorn<T,3>(xz, yz,zz);
				u0 = v0;
				normalize(u0);
				u1 = v1 - ((v1*u0)/(u0*u0))*u0;
				normalize(u1);
				u2 = v2 - ((v2*u0)/(u0*u0))*u0 - ((v2*u1)/(u1*u1))*u1;
				normalize(u2);
				column(0, u0);
				column(1, u0);
				column(2, u0);
			}
			// cast operations
			operator T*() {
				return &xx;
			}
			operator const T*() const {
				return &xx;
			}
	};

	/// Creates a matrix<T,3,3> that represents a rotation around the x axis
	template<typename T>
	void rotx(const T& angle, matrix<T,3,3>& m) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		m.xx = 1.0;
		m.xy = 0.0;
		m.xz = 0.0;
		m.yx = 0.0;
		m.yy = c;
		m.yz = -s;
		m.zx = 0.0;
		m.zy = s;
		m.zz = c;
	}
	/// Creates a matrix<T,3,3> that represents a rotation around the y axis
	template<typename T>
	void roty(const T& angle, tmath::matrix<T,3,3>& m) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		m.xx = c;
		m.xy = 0.0;
		m.xz = s;
		m.yx = 0.0;
		m.yy = 1;
		m.yz = 0;
		m.zx = -s;
		m.zy = 0;
		m.zz = c;
	}
	/// Creates a matrix<T,3,3> that represents a rotation around the z axis
	template<typename T>
	void rotz(const T& angle, tmath::matrix<T,3,3>& m) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		m.xx = c;
		m.xy = -s;
		m.xz = 0.0;
		m.yx = s;
		m.yy = c;
		m.yz = 0.0;
		m.zx = 0.0;
		m.zy = 0.0;
		m.zz = 1.0;
	}
	
	/// Creates a matrix<T,3,3> that represents a rotation around an arbitrary axis
	template<typename T>
	void rot(const T& angle, const tmath::vectorn<T,3>& axis, tmath::matrix<T,3,3>& pM) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		vectorn<T,3> v = axis;
		normalize(v);
		T xy = v.x * v.y;
		T yz = v.y * v.z;
		T zx = v.z * v.x;
		T xs = v.x * s;
		T ys = v.y * s;
		T zs = v.z * s;
		T tt = static_cast<T>(1.0) - c;

		pM.xx = tt * v.x * v.x + c;
		pM.xy = tt * xy - zs;
		pM.xz = tt * zx + ys;

		pM.yx = tt * xy + zs;
		pM.yy = tt * v.y * v.y + c;
		pM.yz = tt * yz - xs;

		pM.zx = tt * zx - ys;
		pM.zy = tt * yz + xs;
		pM.zz = tt * v.z * v.z + c;
	}
	
	/// Calculate the inverse of a matrix
	template<typename T>
	const tmath::matrix<T,3,3>& inverse(const tmath::matrix<T,3,3>& m, tmath::matrix<T,3,3>& mdst) {
		T detinv;
		T det = determinant(m);
		if ( fabs( det ) < static_cast<T>(0.0005) ) {
			mdst = tmath::matrix<T,3,3>();
			return mdst;
		}
		detinv = static_cast<T>(1.0)/det;
		mdst[0] = (m.zz*m.yy - m.zy*m.yz)*detinv;
		mdst[1] = -(m.zz*m.xy - m.zy*m.xz)*detinv;
		mdst[2] = (m.yz*m.xy - m.yy*m.xz)*detinv;
		mdst[3] = -(m.zz*m.yx - m.zx*m.yz)*detinv;
		mdst[4] = (m.zz*m.xx - m.zx*m.xz)*detinv;
		mdst[5] = -(m.yz*m.xx - m.yx*m.xz)*detinv;
		mdst[6] = (m.zy*m.yx - m.zx*m.yy)*detinv;
		mdst[7] = 	-(m.zy*m.xx - m.zx*m.xy)*detinv;
		mdst[8] = (m.yy*m.xx - m.yx*m.xy)*detinv;
		return mdst;
	}

	/// Calculate the determinant of a matrix
	template<typename T>
	T determinant(const tmath::matrix<T,3,3>& m) {
		T det;
		det = m[0] * ( m.yy * m.zz - m.yz * m.zy )
		      - m[3] * ( m.zz * m.xy - m.zy * m.xz )
		      + m[7] * ( m.yz * m.xy - m.yy * m.xz );
		return det;
	}
	
	/// Transpose the matrix<T,3,3>
	template<typename T>
	const tmath::matrix<T,3,3> transpose(const tmath::matrix<T,3,3>& m) {
		return tmath::matrix<T,3,3>(	m.xx, m.yx, m.zx,
		                              m.xy, m.yy, m.zy,
		                              m.xz, m.yz, m.zz);
	}
	
/// Transpose the matrix<T,3,3>
	template<typename T>
	void transpose(const tmath::matrix<T,3,3>& m, tmath::matrix<T,3,3>& rm) {
		rm.xx = m.xx;
		rm.yy = m.yy;
		rm.zz = m.zz;
		rm.yx = m.xy;
		rm.xy = m.yx;
		rm.zx = m.xz;
		rm.xz = m.zx;
		rm.zy = m.yz;
		rm.yz = m.zy;
	}

	/// Make a null matrix<T,3,3>
	template<typename T>
	inline tmath::matrix<T,3,3>& null(tmath::matrix<T,3,3>& mat) {
		mat.xx = static_cast<T>(0.0);
		mat.xy = static_cast<T>(0.0);
		mat.xz = static_cast<T>(0.0);
		mat.yx = static_cast<T>(0.0);
		mat.yy = static_cast<T>(0.0);
		mat.yz = static_cast<T>(0.0);
		mat.zx = static_cast<T>(0.0);
		mat.zy = static_cast<T>(0.0);
		mat.zz = static_cast<T>(0.0);
		return mat;
	}

	/// Make a identity matrix<T,3,3>
	template<typename T>
	inline tmath::matrix<T,3,3>& identity(tmath::matrix<T,3,3>& mat) {
		mat.xx = static_cast<T>(1.0);
		mat.xy = static_cast<T>(0.0);
		mat.xz = static_cast<T>(0.0);
		mat.yx = static_cast<T>(0.0);
		mat.yy = static_cast<T>(1.0);
		mat.yz = static_cast<T>(0.0);
		mat.zx = static_cast<T>(0.0);
		mat.zy = static_cast<T>(0.0);
		mat.zz = static_cast<T>(1.0);
		return mat;
	}
	
	
	template< typename T>
	void convert(const tmath::matrix<T, 3, 3>& rot, tmath::quaternion<T>& quat) {
		quat.w = (rot.xx + rot.yy + rot.zz + 1.0f) / 4.0f;
		quat.x = (rot.xx - rot.yy - rot.zz + 1.0f) / 4.0f;
		quat.y = (-rot.xx + rot.yy - rot.zz + 1.0f) / 4.0f;
		quat.z = (-rot.xx - rot.yy + rot.zz + 1.0f) / 4.0f;
		if(quat.w < 0.0f) quat.w = 0.0f;
		if(quat.x < 0.0f) quat.x = 0.0f;
		if(quat.y < 0.0f) quat.y = 0.0f;
		if(quat.z < 0.0f) quat.z = 0.0f;
		quat.w = sqrt(quat.w);
		quat.x = sqrt(quat.x);
		quat.y = sqrt(quat.y);
		quat.z = sqrt(quat.z);
		if(quat.w >= quat.x && quat.w >= quat.y && quat.w >= quat.z) {
			quat.w *= +1.0f;
			quat.x *= sign(rot.zy - rot.yz);
			quat.y *= sign(rot.xz - rot.zx);
			quat.z *= sign(rot.yx - rot.xy);
		} else if(quat.x >= quat.w && quat.x >= quat.y && quat.x >= quat.z) {
			quat.w *= sign(rot.zy - rot.yz);
			quat.x *= +1.0f;
			quat.y *= sign(rot.yx + rot.xy);
			quat.z *= sign(rot.xz + rot.zx);
		} else if(quat.y >= quat.w && quat.y >= quat.x && quat.y >= quat.z) {
			quat.w *= sign(rot.xz - rot.zx);
			quat.x *= sign(rot.yx + rot.xy);
			quat.y *= +1.0f;
			quat.z *= sign(rot.zy + rot.yz);
		} else if(quat.z >= quat.w && quat.z >= quat.x && quat.z >= quat.y) {
			quat.w *= sign(rot.yx - rot.xy);
			quat.x *= sign(rot.zx + rot.xz);
			quat.y *= sign(rot.zy + rot.yz);
			quat.z *= +1.0f;
		} else {
			// Some problems..
		}
		normalize(quat);
	}
	/// Convert a matrix3 into a matrix4
	template<typename T>
	inline const tmath::matrix<T,4,4>&  convert(const tmath::matrix<T,3,3>& m1, tmath::matrix<T,4,4>& m2) {
		m2.xx = m1.xx; m2.xy = m1.xy; m2.xz = m1.xz; m2.xw = static_cast<T>(0.0);
		m2.yx = m1.yx; m2.yy = m1.yy; m2.yz = m1.yz; m2.yw = static_cast<T>(0.0);
		m2.zx = m1.zx; m2.zy = m1.zy; m2.zz = m1.zz; m2.zw = static_cast<T>(0.0);
		m2.wx = static_cast<T>(0.0); m2.wy = static_cast<T>(0.0); m2.wz = static_cast<T>(0.0); m2.ww = static_cast<T>(1.0);
		return m2;
	}
	
		template<typename T>
	std::ifstream& operator>>(std::ifstream& in, tmath::matrix<T,3,3>& m) {
		in >> m.xx; in >> m.xy; in >> m.xz;
		in >> m.yx; in >> m.yy; in >> m.yz;
		in >> m.zx; in >> m.zy; in >> m.zz;
		return in;
	}
	
		/// Convert a matrix3 into a matrix3
	template<typename T>
	matrix<T,3,3>&  convert(const matrix<T,4,4>& m1, matrix<T,3,3>& m2) {
		m2.xx = m1.xx;
		m2.xy = m1.xy;
		m2.xz = m1.xz;
		m2.yx = m1.yx;
		m2.yy = m1.yy;
		m2.yz = m1.yz;
		m2.zx = m1.zx;
		m2.zy = m1.zy;
		m2.zz = m1.zz;
		return m2;
	}
}

#endif

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

#ifndef MATRIX4_H
#define MATRIX4_H

#include <string.h>

namespace tmath {

	/**
		@author 		Cengiz Terzibas
	*/

	template<typename T>
	class matrix<T,4,4> {
	public:
		
			typedef T value_type;

				T xx, xy, xz, xw;
				T yx, yy, yz, yw;
				T zx, zy, zz, zw;
				T wx, wy, wz, ww;

			matrix<T,4,4>(const T  _a0, const T  _a1, const T  _a2, const T  _a3,
			              const T  _a4, const T  _a5, const T  _a6, const T  _a7,
			              const T  _a8, const T  _a9, const T _a10, const T _a11,
			              const T _a12, const T _a13, const T _a14, const T _a15) :
				xx(_a0), xy(_a1), xz(_a2), xw(_a3),
				yx(_a4), yy(_a5), yz(_a6), yw(_a7),
				zx(_a8), zy(_a9), zz(_a10), zw(_a11),
				wx(_a12), wy(_a13), wz(_a14), ww(_a15) {}

			// constructors
			matrix<T,4,4>() {
				memset(&xx, 0, sizeof(T)*16);
			}

			// assignment operations
			inline matrix<T,4,4>& operator+=(const matrix<T,4,4>& m) {
				xx += m.xx; xy += m.xy; xz += m.xz; xw += m.xw; 
				yx += m.yx; yy += m.yy; yz += m.yz; yw += m.yw;
				zx += m.zx; zy += m.zy; zz += m.zz; zw += m.zw;
				wx += m.wx; wy += m.wy; wz += m.wz; ww += m.ww;
				return *this;
			}

			inline matrix<T,4,4>& operator-=(const matrix<T,4,4>& m) {
				xx -= m.xx; xy -= m.xy; xz -= m.xz; xw -= m.xw;
				yx -= m.yx; yy -= m.yy; yz -= m.yz; yw -= m.yw;
				zx -= m.zx; zy -= m.zy; zz -= m.zz; zw -= m.zw;
				wx -= m.wx; wy -= m.wy; wz -= m.wz; ww -= m.ww;
				return *this;
			}

			inline matrix<T,4,4>& operator*=(const T& num) {
				xx *= num; xy *= num; xz *= num; xw *= num;
				yx *= num; yy *= num; yz *= num; yw *= num;
				zx *= num; zy *= num; zz *= num; zw *= num;
				wx *= num; wy *= num; wz *= num; ww *= num;
				return *this;
			}

			inline matrix<T,4,4>& operator/=(const T& num) {
				T tmp = 1.0/num;
				xx *= tmp; xy *= tmp; xz *= tmp; xw *= tmp;
				yx *= tmp; yy *= tmp; yz *= tmp; yw *= tmp;
				zx *= tmp; zy *= tmp; zz *= tmp; zw *= tmp;
				wx *= tmp; wy *= tmp; wz *= tmp; ww *= tmp;
				return *this;
			}

			// stream operations
			friend std::ostream& operator<<(std::ostream& out, const matrix<T,4,4>& m) {
				out << m.xx << " " << m.xy << " " << m.xz << " " << m.xw << std::endl;
				out << m.yx << " " << m.yy << " " << m.yz << " " << m.yw << std::endl;
				out << m.zx << " " << m.zy << " " << m.zz << " " << m.zw << std::endl;
				out << m.wx << " " << m.wy << " " << m.wz << " " << m.ww << std::endl;
				return out;
			}

			// comparison operations
			inline const bool operator == (const matrix<T,4,4>& m) const {
				return (xx == m.xx && xy == m.xy && xz == m.xz && xw == m.xw &&
				        yx == m.yx && yy == m.yy && yz == m.yz && yw == m.yw &&
				        zx == m.zx && zy == m.zy && zz == m.zz && zw == m.zw &&
				        wx == m.wx && wy == m.wy && wz == m.wz && ww == m.ww);
			}

			inline const bool operator != (const matrix<T,4,4>& m) const {
				return !(m == *this);
			}

			// unary operations
			inline const matrix<T,4,4> operator - () const {
				return matrix<T,4,4>(-xx, -xy, -xz, -xw,
				                     -yx, -yy, -yz, -yw,
				                     -zx, -zy, -zz, -zw,
				                     -wx, -wy, -wz, -ww);
			}

			// binary operations
			inline matrix<T,4,4> operator+(const matrix<T,4,4>& m) {
				return matrix<T,4,4>(xx + m.xx, xy + m.xy, xz + m.xz, xw + m.xw,
				                     yx + m.yx, yy + m.yy, yz + m.yz, yw + m.yw,
				                     zx + m.zx, zy + m.zy, zz + m.zz, zw + m.zw,
				                     wx + m.wx, wy + m.wy, wz + m.wz, ww + m.ww);
			}

			inline matrix<T,4,4> operator-(const matrix<T,4,4>& m) {
				return matrix<T,4,4>(xx - m.xx, xy - m.xy, xz - m.xz, xw - m.xw,
				                     yx - m.yx, yy - m.yy, yz - m.yz, yw - m.yw,
				                     zx - m.zx, zy - m.zy, zz - m.zz, zw - m.zw,
				                     wx - m.wx, wy - m.wy, wz - m.wz, ww - m.ww);
			}

			inline const matrix<T,4,4> operator*(const T& num) const {
				return matrix<T,4,4>(xx * num, xy * num, xz * num, xw * num,
				                     yx * num, yy * num, yz * num, yw * num,
				                     zx * num, zy * num, zz * num, zw * num,
				                     wx * num, wy * num, wz * num, ww * num);
			}

			friend inline const matrix<T,4,4> operator*(const T &s, const matrix<T,4,4> &m) {
				return m * s;
			}

		
			// Column-Row Multiplication.
			inline matrix<T,4,4> operator*(const matrix<T,4,4>& m) {
				return matrix<T,4,4>(xx * m.xx + yx * m.xy + zx * m.xz + wx * m.xw,
				                     xy * m.xx + yy * m.xy + zy * m.xz + wy * m.xw,
				                     xz * m.xx + yz * m.xy + zz * m.xz + wz * m.xw,
				                     xw * m.xx + yw * m.xy + zw * m.xz + ww * m.xw,

				                     xx * m.yx + yx * m.yy + xz * m.yz + wx * m.yw,
				                     xy * m.yx + yy * m.yy + zy * m.yz + wy * m.yw,
				                     xz * m.yx + yz * m.yy + zz * m.yz + wz * m.yw,
				                     xw * m.yx + yw * m.yy + zw * m.yz + ww * m.yw,

														 xx * m.zx + yx * m.zy + zx * m.zz + wx * m.zw,
				                     xy * m.zx + yy * m.zy + zy * m.zz + wy * m.zw,
				                     xz * m.zx + yz * m.zy + zz * m.zz + wz * m.zw,
				                     xw * m.zx + yw * m.zy + zw * m.zz + ww * m.zw,

														 xx * m.wx + yx * m.wy + zx * m.wz + wx * m.ww,
				                     xy * m.wx + yy * m.wy + zy * m.wz + wy * m.ww,
				                     xz * m.wx + yz * m.wy + zz * m.wz + wz * m.ww,
				                     xw * m.wx + yw * m.wy + zw * m.wz + ww * m.ww);

			}

			inline const vectorn<T,4> operator*(const vectorn<T,4>& v) {
				return vectorn<T,4>(v.x*xx + v.y*yx + v.z*zx + v.w*wx,
				                    v.x*xy + v.y*yy + v.z*zy + v.w*wy,
				                    v.x*xz + v.y*yz + v.z*zz + v.w*wz,
				                    v.x*xw + v.y*yw + v.z*zw + v.w*ww);

			}

			inline const matrix<T,4,4> operator/(const T& num) const {
				T val = 1.0/num;
				return matrix<T,4,4>(xx * val, xy * val, xz * val, xw * val,
				                     yx * val, yy * val, yz * val, yw * val,
				                     zx * val, zy * val, zz * val, zw * val,
				                     wx * val, wy * val, wz * val, ww * val);
			}

			// indexing operations
			void row(unsigned int pRow, const vectorn<T,4>& pV) {
				unsigned idx = 4*pRow;
				(*this)[idx]			= pV.x;
				(*this)[idx + 1]	= pV.y;
				(*this)[idx + 2]	= pV.z;
				(*this)[idx + 3]	= pV.w;
			}

			vectorn<T,4> row(unsigned int pRow) {
				unsigned idx = 4*pRow;
				return vectorn<T,4>((*this)[idx],(*this)[idx+1],(*this)[idx+2],(*this)[idx+3]);
			}

			void column(unsigned int pColumn, const vectorn<T,4>& pV) {
				(*this)[pColumn]	= pV.x;
				(*this)[pColumn + 4]	= pV.y;
				(*this)[pColumn + 8]	= pV.z;
				(*this)[pColumn + 12]	= pV.w;
			}

			vectorn<T,4> column(unsigned int pColumn) {
				return vectorn<T,4>((*this)[pColumn],(*this)[pColumn+4],(*this)[pColumn+8],(*this)[pColumn+12]);
			}

			inline const vectorn<T,4> row(int idx) {
				return vectorn<T,4>(*(&xx + 4*idx),*(&xx + 4*idx +1),*(&xx + 4*idx +2),*(&xx + 4*idx +3));
			}

			inline const vectorn<T,4> column(int idx) {
				return vectorn<T,4>(*(&xx + idx),*(&xx + idx+ 4),*(&xx + idx + 8),*(&xx + idx + 12));
			}

			// cast operations
			operator T*() {
				return &xx;
			}

			operator const T*() const {
				return &xx;
			}
			
			T& at(unsigned int row, unsigned int column) {
				return xx[row][column];
			}
	};

	/// Transpose a matrix.
	template<typename T>
	const matrix<T,4,4> transpose(const matrix<T,4,4>& m) {
		return matrix<T,4,4>(	m.xx, m.yx, m.zx, m.wx,
													m.xy, m.yy, m.zy, m.wy,
													m.xz, m.yz, m.zz, m.wz,
													m.xw, m.yw, m.zw, m.ww);
	}

	/// Calculates the determinant of a matrix.
	template<typename T>
	T determinant(const matrix<T,4,4>& m) {
		T det;
		det = m[0] * m[5] * m[10];
		det += m[4] * m[9] * m[2];
		det += m[8] * m[1] * m[6];
		det -= m[8] * m[5] * m[2];
		det -= m[4] * m[1] * m[10];
		det -= m[0] * m[9] * m[6];
		return det;
	}

	/// Creates a translation matrix.
	template<typename T>
	inline matrix<T,4,4>& translate(const T x, const T y, const T z, matrix<T,4,4>& m) {
		m[0]	= 1.0f; m[5] 	= 1.0f; m[10]	= 1.0f; m[15] = 1.0f;
		m[12]	= x; 		m[13] = y; 		m[14]	= z;
		return m;
	}

	/// Creates a translation matrix.
	template<typename T>
	inline matrix<T,4,4>&  translate(const vectorn<T,3>& v, matrix<T,4,4>& m) {
		return translate(v.x, v.y, v.z, m);
	}

	/// Creates a scale matrix.
	template<typename T>
	inline matrix<T,4,4>& scale(const T sx, const T sy, const T sz, matrix<T,4,4>& m) {
		m[0]  = sx; m[5]  = sy; m[10] = sz; m[15] = 1.0f;
		return m;
	}
	
	/// Creates a scale matrix.
	template<typename T>
	inline matrix<T,4,4>& scale(const vectorn<T,3>& v, matrix<T,4,4>& m) {
		return scale(v.x, v.y, v.z, m);
	}
	
	/// Creates a matrix<T,4,4> that represents a rotation around the x axis.
	template<typename T>
	matrix<T,4,4>& rotate_x(const T& angle, matrix<T,4,4>& m) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		m[5] 	=  c;  m[6] = s;
		m[9] 	= -s;  m[10] = c;
		m[0] 	= 1.0; m[15] = 1.0;
		return m;
	}

	/// Creates a matrix<T,4,4> that represents a rotation around the y axis.
	template<typename T>
	matrix<T,4,4>& rotate_y(const T& angle, matrix<T,4,4>& m) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		m[0] = c;   m[2] = -s;
		m[8] = s;   m[10] = c;
		m[5] = 1.0; m[15] = 1.0;
		return m;
	}
	
	/// Creates a matrix<T,4,4> that represents a rotation around the z axis.
	template<typename T>
	matrix<T,4,4>& rotate_z(const T& angle, matrix<T,4,4>& m) {
		T rad = d2r(angle);
		T c = cos(rad);
		T s = sin(rad);
		m[0] =  c; m[1] = s;
		m[4] = -s; m[5] = c;
		m[10] = 1.0; m[15] = 1.0;
		return m;
	}
	
	/// Creates a perspective frustum matrix using left, right, bottom, top near and far.
	template<typename T>
	matrix<T,4,4>& frustum(T left_plane, T right_plane, T bottom_plane, T top_plane, T near_plane, T far_plane, matrix<T,4,4>& mat) {
		T d = (right_plane-left_plane);
		T c = (top_plane-bottom_plane);
		mat[0] = 2.0f*near_plane/d; mat[1] = 0.0f; mat[2] = 0.0f; mat[3] = 0.0f;
		mat[4] = 0.0f; mat[5] = 2.0f*near_plane/c; mat[6] = 0.0f; mat[7] = 0.0f;
		mat[8] =(right_plane+left_plane)/d; mat[9] = (top_plane+bottom_plane)/c; mat[10] = -(far_plane+near_plane)/(far_plane-near_plane); mat[11] = -1.0f;
		mat[12] = 0.0f; mat[13] = 0.0f; mat[14] = -2.0f*far_plane*near_plane/(far_plane-near_plane); mat[15] = 0.0f;
		return mat;
	}
	
	/// Creates ortho matrix using left, right, bottom, top, near and far.
	template<typename T>
	matrix<T, 4, 4>& ortho(T left, T right, T bottom, T top, T near_plane_distance, T far_plane_distance, matrix<T, 4, 4>& mat) {
		T d = (right-left);
		T c = (top-bottom);
		mat[0] = 2.0f/d; mat[1] = 0.0f; mat[2] = 0.0f; mat[3] = 0.0f;
		mat[4] = 0.0f; mat[5] = 2.0f/c; mat[6] = 0.0f; mat[7] = 0.0f;
		mat[8] = 0.0f; mat[9] = 0.0f; mat[10] = -2.0f / (far_plane_distance - near_plane_distance); mat[11] = 0.0f;
		mat[12] = -(right + left) / d; mat[13] = -(top + bottom) / c; mat[14] = -(far_plane_distance + near_plane_distance) / (far_plane_distance - near_plane_distance); mat[15] = 1.0f;
		return mat;
	}
	
	/// Creates a perspective matrix using fov, aspect, near and far.
	template<typename T>
	matrix<T,4,4>& perspective(T fov, T aspect, T near_plane_distance, T far_plane_distance, matrix<T,4,4>& mat) {
		T f = 1.0/tan(fov*0.5);
		T d = (near_plane_distance - far_plane_distance);
		mat[0]  = f/aspect; mat[1]  = 0.0f; mat[2]  = 0.0f;           mat[3] = 0.0f;
		mat[4]  = 0.0;      mat[5]  = f;    mat[6]  = 0.0f;           mat[7] = 0.0f;
		mat[8] = 0.0;      mat[9] = 0.0f; mat[10] = (far_plane_distance + near_plane_distance) / d; mat[11] = (2.0*far_plane_distance * near_plane_distance) / d;
		mat[12] = 0.0;      mat[13] = 0.0;  mat[14] = -1.0;           mat[15] = 0.0;
		return mat;
	}
	
	/// Creates a rotation matrix around a free axis.
	template<typename T>
	matrix<T,4,4>& rotate(const T& angle, const vectorn<T,3>& axis, matrix<T,4,4>& mat) {
		T rad = d2r(angle);
		T c = static_cast<T>(cos(rad));
		T s = static_cast<T>(sin(rad));
		vectorn<T,3> v = axis;
		normalize(v);
		T xy = v.x * v.y;
		T yz = v.y * v.z;
		T zx = v.z * v.x;
		T xs = v.x * s;
		T ys = v.y * s;
		T zs = v.z * s;
		T tt = static_cast<T>(1.0) - c;

		mat[0] = tt * v.x * v.x + c; mat[1] = tt * xy + zs; mat[2] = tt * zx - ys; mat[3] = 0.0;
		mat[4] = tt * xy - zs; mat[5] = tt * v.y * v.y + c; mat[6] = tt * yz + xs; mat[7] = 0.0;
		mat[8] = tt * zx + ys; mat[9] = tt * yz - xs; mat[10] = tt * v.z * v.z + c; mat[11] = 0.0;
		mat[12] = 0.0; mat[13] = 0.0; mat[14] = 0.0; mat[15] = 1.0;
		return mat;
	}


	/// Makes a zero matrix<T,4,4>
	template<typename T>
	matrix<T,4,4>& zero(matrix<T,4,4>& mat) {
		memset(mat, 0, sizeof(matrix<T,4,4>));
		return mat;
	}
	
	/// Make a identity matrix<T,4,4>
	template<typename T>
	matrix<T,4,4>& identity(matrix<T,4,4>& mat) {
		mat[0] 	= 1.0; mat[1] 	= 0.0; mat[2] 	= 0.0; mat[3] 	= 0.0;
		mat[4] 	= 0.0; mat[5] 	= 1.0; mat[6] 	= 0.0; mat[7] 	= 0.0;
		mat[8] 	= 0.0; mat[9] 	= 0.0; mat[10] 	= 1.0; mat[11] 	= 0.0;
		mat[12] = 0.0; mat[13] 	= 0.0; mat[14] 	= 0.0; mat[15] 	= 1.0;
		return mat;
	}
	
	/// Converts matrix into a quaterion.
	template<typename T>
	quaternion<T>& convert(const matrix<T,4,4>& mat, quaternion<T>& quat) {
		quat.w = (mat[0] + mat[5] + mat[10] + 1.0f) / 4.0f;
		quat.x = (mat[0] - mat[5] - mat[10] + 1.0f) / 4.0f;
		quat.y = (-mat[0] + mat[5] - mat[10] + 1.0f) / 4.0f;
		quat.z = (-mat[0] - mat[5] + mat[10] + 1.0f) / 4.0f;
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
			quat.x *= sign(mat[6] - mat[9]);
			quat.y *= sign(mat[8] - mat[2]);
			quat.z *= sign(mat[1] - mat[4]);
		} else if(quat.x >= quat.w && quat.x >= quat.y && quat.x >= quat.z) {
			quat.w *= sign(mat[6] - mat[9]);
			quat.x *= +1.0f;
			quat.y *= sign(mat[1] + mat[4]);
			quat.z *= sign(mat[8] + mat[2]);
		} else if(quat.y >= quat.w && quat.y >= quat.x && quat.y >= quat.z) {
			quat.w *= sign(mat[8] - mat[2]);
			quat.x *= sign(mat[1] + mat[4]);
			quat.y *= +1.0f;
			quat.z *= sign(mat[6] + mat[9]);
		} else if(quat.z >= quat.w && quat.z >= quat.x && quat.z >= quat.y) {
			quat.w *= sign(mat[1] - mat[4]);
			quat.x *= sign(mat[2] + mat[8]);
			quat.y *= sign(mat[6] + mat[9]);
			quat.z *= +1.0f;
		} else {
			// Some problems..
		}
		normalize(quat);
		return quat;
	}

/// Calculates the inverse of a matrix<T,4,4>
	template<typename T>
	matrix<T,4,4> inverse(const matrix<T,4,4>& m) {
		tmath::matrix<T,4,4> mat;
		T tmp[12];
		T src[16];

		for ( int i = 0; i < 4; i++) {
			src[i] = m[i*4];
			src[i + 4] = m[i*4 + 1];
			src[i + 8] = m[i*4 + 2];
			src[i + 12] = m[i*4 + 3];
		}

		tmp[0] = src[10] * src[15];
		tmp[1] = src[11] * src[14];
		tmp[2] = src[9] * src[15];
		tmp[3] = src[11] * src[13];
		tmp[4] = src[9] * src[14];
		tmp[5] = src[10] * src[13];
		tmp[6] = src[8] * src[15];
		tmp[7] = src[11] * src[12];
		tmp[8] = src[8] * src[14];
		tmp[9] = src[10] * src[12];
		tmp[10] = src[8] * src[13];
		tmp[11] = src[9] * src[12];

		// calculate first 8 elements (cofactors)
		mat.xx = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
		mat.xx -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
		mat.xy = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
		mat.xy -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
		mat.xz = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
		mat.xz -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
		mat.xw = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
		mat.xw -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
		mat.yx = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
		mat.yx -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
		mat.yy = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
		mat.yy -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
		mat.yz = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
		mat.yz -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
		mat.yw = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
		mat.yw -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

		tmp[0] = src[2]*src[7];
		tmp[1] = src[3]*src[6];
		tmp[2] = src[1]*src[7];
		tmp[3] = src[3]*src[5];
		tmp[4] = src[1]*src[6];
		tmp[5] = src[2]*src[5];
		tmp[6] = src[0]*src[7];
		tmp[7] = src[3]*src[4];
		tmp[8] = src[0]*src[6];
		tmp[9] = src[2]*src[4];
		tmp[10] = src[0]*src[5];
		tmp[11] = src[1]*src[4];


		mat.zx = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
		mat.zx -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
		mat.zy = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
		mat.zy -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
		mat.zz = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
		mat.zz-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
		mat.zw = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
		mat.zw-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
		mat.wx = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
		mat.wx-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
		mat.wy = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
		mat.wy-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
		mat.wz = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
		mat.wz-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
		mat.ww = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
		mat.ww-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

		// calculate determinant
		T det=src[0]*mat.xx+src[1]*mat.xy+src[2]*mat.xz+src[3]*mat.xw;
		if ( fabs( det ) < static_cast<T>(0.0005) ) {
			return tmath::matrix<T,4,4>();
		}

		det = static_cast<T>(1.0)/det;
		for ( int j = 0; j < 16; j++)
			mat[j] *= det;
			
		return mat;
	}	
	
	
	/// Creates a look at matrix.
	template<typename T>
	matrix<T,4,4>& look_at(vectorn<T,3>& position, vectorn<T,3>& target, vectorn<T,3>& up, matrix<T,4,4>& m) {
		vectorn<T,3> forward, side;

		// Calculate the direction of the view.
		forward = target - position;
		tmath::normalize(forward);

		// Calculate the positive right coordinate
		tmath::cross(forward, up, side);
		tmath::normalize(side);

		// Now we must recompute the up vector to ajust
		// it to the new right vector.
		tmath::cross(side, forward, up);
		normalize(up);

		// Fill the right information into the matrix
		 m[0] = side.x; m[1] = side.y; m[2] = side.z; m[3] = 0.0;
		 m[4] = up.x;		m[5] = up.y	;	 m[6] = up.z;		m[7] = 0.0;
		 m[8] = forward.x; m[9] = forward.y; m[10] = forward.z; m[11] = 0.0;
		 m[12] = position.x; m[13] = -position.y; m[14] = -position.z; m[15] = 1.0f;

		return m;
	}
	
	
	template<typename T>
	std::ifstream& operator>>(std::ifstream& in, matrix<T,4,4>& m) {
		in >> m.xx; in >> m.xy; in >> m.xz; in >> m.xw;
		in >> m.yx; in >> m.yy; in >> m.yz; in >> m.yw;
		in >> m.zx; in >> m.zy; in >> m.zz; in >> m.zw;
		in >> m.wx; in >> m.wy; in >> m.wz; in >> m.ww;
		return in;
	}

}

#endif

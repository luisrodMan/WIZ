#include "WIZ/math.h"
#include <cmath>

#include <vector>
#include <string>

/*

	v e c t o r

*/

Vector2::Vector2() : Vector2(0, 0) {}
		
Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}
void Vector2::set(float x, float y) {
	this->x = x;
	this->y = y;
}

void Vector2::normalize() {
	float lenght = sqrt(x * x + y * y);
	if (lenght == 0)
		set(0, 0);
	else {
		x /= lenght;
		y /= lenght;
	}
}

float Vector2::dist(float x, float y) {
	x -= this->x;
	y -= this->y;
	return sqrt(x * x + y * y);
}

float Vector2::dist(Vector2 &other) {
	float x = other.x;
	float y = other.y;
	x -= this->x;
	y -= this->y;
	return sqrt(x * x + y * y);	
}

float Vector2::distSquare(float x, float y) {
	x -= this->x;
	y -= this->y;
	return x * x + y * y;
}

float Vector2::distSquare(Vector2 &other) {
	float xx = x - other.x;
	float yy = y - other.y;
	return xx * xx + yy * yy;
}


float Vector2::dot(Vector2 &o) {
	
	return x * o.x + y * o.y;
	
}
		
void Vector2::add(float x2, float y2) {
	x += x2;
	y += y2;
}


void Vector2::add(Vector2 &v) {
	x += v.x;
	y += v.y;
}


void Vector2::mult(float v) {
	x *= v;
	y *= v;
}

void Vector2::mult(float x2, float y2) {
	x *= x2;
	y *= y2;
}


void Vector2::sub(float x2, float y2) {
	x -= x2;
	y -= y2;
}


void Vector2::sub(const Vector2 &v) {
	x -= v.x;
	y -= v.y;
}

void Vector2::setLenght(float lenght) {
	normalize();
	x *= lenght;
	y *= lenght;
}

void Vector2::rotate(float angle) {
	angle = -angle * 3.141589999999f / 180;
	double cos = std::cos(angle);
	double sin = std::sin(angle);
	float temp = x;
	x = (float)(cos * x - sin * y);
	y = (float)(sin * temp + cos * y);
}

float Vector2::toAngle() {
	return atan2(-y, x) * 180 / 3.141589999999f;
}

float Vector2::getLenght() {
	return sqrt(x * x + y * y);
}

float Vector2::getLenght2() {
	return x * x + y * y;
}

// OVERLOADING
Vector2 Vector2::operator*(float v) {
	Vector2 res;
	res.x = x * v;
	res.y = y * v;
	return res;
}
Vector2 &Vector2::operator*=(float v) {
	x *= v;
	y *= v;
	return *this;
}

Vector2 Vector2::operator+(Vector2 &v2) {
	Vector2 res;
	res.x = x + v2.x;
	res.y = y + v2.y;
	return res;
}
Vector2 Vector2::operator-(Vector2 &v2) {
	Vector2 res;
	res.x = x - v2.x;
	res.y = y - v2.y;
	return res;
}

Vector2 &Vector2::operator+=(Vector2 &v2) {
	x += v2.x;
	y += v2.y;
	return *this;
}
Vector2 &Vector2::operator-=(Vector2 &v2) {
	x -= v2.x;
	y -= v2.y;
	return *this;
}







Vector::Vector() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}


Vector::Vector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}



/*

	M A T R I X

*/


Matrix::Matrix() {
	setToIdentity();
}

Matrix::Matrix(float elements[]) {
	set(elements);
}

Matrix::Matrix(float m11, float m12, float m13, float m14,
			  float m21, float m22, float m23, float m24,
			  float m31, float m32, float m33, float m34,
			  float m41, float m42, float m43, float m44)
{
	M11 = m11; M12 = m12; M13 = m13; M14 = m14;
	M21 = m21; M22 = m22; M23 = m23; M24 = m24;
	M31 = m31; M32 = m32; M33 = m33; M34 = m34;
	M41 = m41; M42 = m42; M43 = m43; M44 = m44;
}

Vector Matrix::transform(float x, float y, float z) {
	Vector v;
	transform(v);
	return v;
}

void Matrix::transform(Vector &v) {
	float x = v.x; float y = v.y; float z = v.z;
	v.x = M11 * x + M12 * y + M13 * z  +  M14;
	v.y	= M21 * x + M22 * y + M23 * z  +  M24;
	v.z = M31 * x + M32 * y + M33 * z  +  M34;
}


Matrix* Matrix::transpose() {
	float x = M12;
	M12 = M21;
	M21 = x;
	x = M13;
	M13 = M31;
	M31 = x;
	x = M14;
	M14 = M41;
	M41 = x;
	
	x = M23;
	M23 = M32;
	M32 = x;
	x = M24;
	M24 = M42;
	M42 = x;
	
	x = M34;
	M34 = M43;
	M43 = x;
	
	return this;
}

void Matrix::transform(Matrix &m, Matrix &r) {
	
	r.M11 = M11 * m.M11 + M12 * m.M21 + M13 * m.M31 + M14 * m.M41; 
	r.M12 = M11 * m.M12 + M12 * m.M22 + M13 * m.M32 + M14 * m.M42; 
	r.M13 = M11 * m.M13 + M12 * m.M23 + M13 * m.M33 + M14 * m.M43; 
	r.M14 = M11 * m.M14 + M12 * m.M24 + M13 * m.M34 + M14 * m.M44; 
	
	//fila 2
	r.M21 = M21 * m.M11 + M22 * m.M21 + M23 * m.M31 + M24 * m.M41; 
	r.M22 = M21 * m.M12 + M22 * m.M22 + M23 * m.M32 + M24 * m.M42; 
	r.M23 = M21 * m.M13 + M22 * m.M23 + M23 * m.M33 + M24 * m.M43; 
	r.M24 = M21 * m.M14 + M22 * m.M24 + M23 * m.M34 + M24 * m.M44;
	
	//fila 3
	r.M31 = M31 * m.M11 + M32 * m.M21 + M33 * m.M31 + M34 * m.M41; 
	r.M32 = M31 * m.M12 + M32 * m.M22 + M33 * m.M32 + M34 * m.M42; 
	r.M33 = M31 * m.M13 + M32 * m.M23 + M33 * m.M33 + M34 * m.M43; 
	r.M34 = M31 * m.M14 + M32 * m.M24 + M33 * m.M34 + M34 * m.M44;
	
	// fila 4
	r.M41 = M41 * m.M11 + M42 * m.M21 + M43 * m.M31 + M44 * m.M41; 
	r.M42 = M41 * m.M12 + M42 * m.M22 + M43 * m.M32 + M44 * m.M42; 
	r.M43 = M41 * m.M13 + M42 * m.M23 + M43 * m.M33 + M44 * m.M43; 
	r.M44 = M41 * m.M14 + M42 * m.M24 + M43 * m.M34 + M44 * m.M44;

}

// this * m
 Matrix* Matrix::transform(Matrix &m) {
	//f1 
	//f2
	//f3
	//f4
	// fila 1
	
	Matrix *r = new Matrix();
	
	transform(m, *r);
	
	return r;
}


Matrix* Matrix::inverse() {
	Matrix *m = new Matrix(M11, M21, M31, M41,
							M12, M22, M32, M42, 
							M13, M23, M33, M43, 
							M14, M24, M34, M44);
	return m;
}




void Matrix::setToIdentity() {
	
	M11 = 1; M12 = 0; M13 = 0; M14 = 0;
	M21 = 0; M22 = 1; M23 = 0; M24 = 0;
	M31 = 0; M32 = 0; M33 = 1; M34 = 0;
	M41 = 0; M42 = 0; M43 = 0; M44 = 1;
	
}


void Matrix::set(float data[])
{
	M11 = data[0];
	M12 = data[1];
	M13 = data[2];
	M14 = data[3];
	M21 = data[4];
	M22 = data[5];
	M23 = data[6];
	M24 = data[7];
	M31 = data[8];
	M32 = data[9];
	M33 = data[10];
	M34 = data[11];
	M41 = data[12];
	M42 = data[13];
	M43 = data[14];
	M44 = data[15];
}

void Matrix::set(Matrix &m) {
	M11 = m.M11;
	M12 = m.M12;
	M13 = m.M13;
	M14 = m.M14;
	M21 = m.M21;
	M22 = m.M22;
	M23 = m.M23;
	M24 = m.M24;
	M31 = m.M31;
	M32 = m.M32;
	M33 = m.M33;
	M34 = m.M34;
	M41 = m.M41;
	M42 = m.M42;
	M43 = m.M43;
	M44 = m.M44;
}




Matrix *Matrix::setTranslate(float x, float y, float z) {
	setToIdentity();
	M14 = x;
	M24 = y;
	M34 = z;
	M44 = 1;
	return this;
}


Matrix *Matrix::setRotationZ(float radians) {
	setToIdentity();
	float cos1 = (float)std::cos(radians);
	float sin1 = (float)(std::sin(radians));
	M11 = cos1; M12 = -sin1;
	M21 = sin1; M22 = cos1;
	return this;
}

// static version
Matrix *Matrix::rotateZ(float radians) {
	Matrix *x = new Matrix();
	x->setRotationZ(radians);
	return x;
}

Matrix* Matrix::setToOrtho(float width, float height, float zNear, float zFar) {
	return setToOrtho(0, width, 0, height, zNear, zFar);
}

Matrix* Matrix::setToOrtho(float left, float right, 
	float bottom, float top, float zNear, float zFar) {
	Matrix *m = this;

	// escalamos de -1 a 1 => por eso dividimos 1 / MitadTamanio

	// m11 = 1.0f / (width / 2); ==> 1.0f / (tamanio) * 2;
	
	
	float invWidth = 1.0f / (right - left);
	float invHeight = 1.0f / (top - bottom);
	float invDepth = 1.0f / (zFar - zNear);

	m->M11 = invWidth * 2;
	m->M22 = invHeight * 2;
	m->M33 = invDepth * -2;

	// estos ultimos valores son para manter la consistencia de -1 a 1
	// deberian de estar en M14, M24 y M34 pero debido a que OpenGL lee columna por columna
	// agregamos estos valores en la columna correspondiente
	
	m->M14 = -((right + left) * invWidth);// x * 1/width;
	m->M24 = -((top + bottom) * invHeight);
	m->M34 = -((zFar + zNear) * invDepth);
	
	return m;
}



float* Matrix::toArray(float result[]) {
	result[0]  = M11; result[1]  = M12; result[2]  = M13; result[3]  = M14;
	result[4]  = M21; result[5]  = M22; result[6]  = M23; result[7]  = M24;
	result[8]  = M31; result[9]  = M32; result[10] = M33; result[11] = M34;
	result[12] = M41; result[13] = M42; result[14] = M43; result[15] = M44;
	return result;
}

/**Return a new array with this matriz�s data read row by row</summary>
float[] Matrix::ToArray()
{
	return ToArray(new float[16]);
}
*/


/**
* populate el array result con los datos de esta matriz leido fila por fila 
* el array resivido minimo debe tener una capacidad de 16 elementos
* de lo contrario se producira una exception en tiempo de ejecucion
*/
float* Matrix::toGLArray(float result[])
{

	// esta es la transpuesta    para open gl xd
	result[0]  = M11; result[1]  = M21; result[2]  = M31; result[3]  = M41;
	result[4]  = M12; result[5]  = M22; result[6]  = M32; result[7]  = M42;
	result[8]  = M13; result[9]  = M23; result[10] = M33; result[11] = M43;
	result[12] = M14; result[13] = M24; result[14] = M34; result[15] = M44;
	return result;
}

/**
 * 
 * Return rotation and scale component of this matrix
 * 
 * @param result
 * @return
 */
float* Matrix::toArray3x3(float result[]) {
	result[0]  = M11; result[1]  = M21; result[2]  = M31; 
	result[3]  = M12; result[4]  = M22; result[5]  = M32; 
	result[6]  = M13; result[7]  = M23; result[8] = M33; 
	return result;
}









//  0  1  2  3
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15

float Matrix::getDeterminant() {
	return M31*M22*M13+
			M21*M32*M13+
			M31*M12*M23-
			M11*M32*M23-
			M21*M12*M33+
			M11*M22*M33;
}

Matrix* Matrix::setInverse(Matrix &m) {
	// Make sure the determinant is non-zero.
	float det = getDeterminant();
	if (det == 0) return nullptr;
	det = 1.0f/det;
	M11 = (-m.M32*m.M23+m.M22*m.M33)*det;
	M21 = (m.M31*m.M23-m.M21*m.M33)*det;
	M31 = (-m.M31*m.M22+m.M21*m.M32)*det;
	M12 = (m.M32*m.M13-m.M12*m.M33)*det;
	M22 = (-m.M31*m.M13+m.M11*m.M33)*det;
	M32 = (m.M31*m.M12-m.M11*m.M32)*det;
	M13 = (-m.M22*m.M13+m.M12*m.M23)*det;
	
	M23 = (+m.M21*m.M13-m.M11*m.M23)*det;
	M33 = (-m.M21*m.M12+m.M11*m.M22)*det;
	M14 = (m.M32*m.M23*m.M14
	-m.M22*m.M33*m.M14
	-m.M32*m.M13*m.M24
	+m.M12*m.M33*m.M24
	+m.M22*m.M13*m.M34
	-m.M12*m.M23*m.M34)*det;
	M24 = (-m.M31*m.M23*m.M14
	+m.M21*m.M33*m.M14
	+m.M31*m.M13*m.M24
	-m.M11*m.M33*m.M24
	-m.M21*m.M13*m.M34
	+m.M11*m.M23*m.M34)*det;
	M34 =(m.M31*m.M22*m.M14
	-m.M21*m.M32*m.M14
	-m.M31*m.M12*m.M24
	+m.M11*m.M32*m.M24
	+m.M21*m.M12*m.M34
	-m.M11*m.M22*m.M34)*det;
	
	
	
	
	return this;
}
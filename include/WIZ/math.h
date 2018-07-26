#ifndef __WIZ_MATH__
#define __WIZ_MATH__


class Vector2 {
	
public:

	float x, y;
	
	Vector2();
	
	Vector2(float x, float y);

	void set(float x, float y);

	void normalize();


	float dist(float x, float y);
	float dist(Vector2 &other);
	float distSquare(float x, float y);
	float distSquare(Vector2 &other);


	float dot(Vector2 &o);
		
	void add(float x2, float y2);


	void add(Vector2 &v);


	void mult(float v);

	void mult(float x2, float y2);


	void sub(float x2, float y2);


	void sub(const Vector2 &v);

	void setLenght(float lenght);

	void rotate(float angle);

	float toAngle();

	float getLenght();

	float getLenght2();

	// OVERLOADING
	Vector2 operator*(float v);
	Vector2 &operator*=(float v);

	Vector2 operator+(Vector2 &v2);
	Vector2 operator-(Vector2 &v2);

	Vector2 &operator+=(Vector2 &v2);
	Vector2 &operator-=(Vector2 &v2);

};


class Vector {
	
	public:
	
		float x, y, z;
		
		Vector();
		
		Vector(float x, float y, float z);
};




class Matrix{
	
	public:
	
		static Matrix *rotateZ(float radians);
		
		float 	 M11, M12, M13, M14,
				 M21, M22, M23, M24,
				 M31, M32, M33, M34,
				 M41, M42, M43, M44;

	Matrix();

	Matrix(float elements[]);

	Matrix(float m11, float m12, float m13, float m14,
				  float m21, float m22, float m23, float m24,
				  float m31, float m32, float m33, float m34,
				  float m41, float m42, float m43, float m44);
				  
				  
	
	void setToIdentity();

	/**
	* Establece los valores para esta matriz apartir de data.
	* Los valores seran almacenados fila por fila leidos secuencialmente
	* Note: La matrix resivida debe tener como minimo un longitud de 16
	*/
	 void set(float data[]);
	

	void set(Matrix &m);
	
	
	Matrix* setToOrtho(float width, float height, float zNear, float zFar);

	Matrix* setToOrtho(float left, float right, 
		float bottom, float top, float zNear, float zFar);
	
	
	Vector transform(float x, float y, float z);
	
	// this * v
	void transform(Vector &v);
	
	Matrix* transpose();
	
	void transform(Matrix &m, Matrix &result);
	
	
	// this * m
	 Matrix* transform(Matrix &m);
	
	
	 Matrix* inverse();
	
	Matrix* setTranslate(float x, float y, float z);
	
	Matrix* setRotationZ(float radians);


	float* toArray(float result[]);

	/**Return a new array with this matriz�s data read row by row</summary>
	 float[] ToArray()
	{
		return ToArray(new float[16]);
	}
	*/
	
	
	/**
	* populate el array result con los datos de esta matriz leido fila por fila 
	* el array resivido minimo debe tener una capacidad de 16 elementos
	* de lo contrario se producira una exception en tiempo de ejecucion
	*/
	 float* toGLArray(float result[]);
	
	/**
	 * 
	 * Return rotation and scale component of this matrix
	 * 
	 * @param result
	 * @return
	 */
	 float* toArray3x3(float result[]);

	
	
	//  0  1  2  3
	//  4  5  6  7
	//  8  9 10 11
	// 12 13 14 15
	
	float getDeterminant();
	
	Matrix* setInverse(Matrix &m);
};


#endif
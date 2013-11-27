//Vectores para representar cosas en dos dimensiones.
struct Vector2D
{
	double x, y;

	//Constructor
	Vector2D(double a = 0.0f, double b = 0.0f):x(a), y(b) {}

	//Overloading de operadores para facilitar el trabajo con vectores.
	//Estos modifican el vector original.
	Vector2D &operator+=(const Vector2D &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	Vector2D &operator-=(const Vector2D &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	Vector2D &operator*=(const double &rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	vector2D &operator/=(const double &rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}
};

//Más Overloading. Estos devuelven otro vector resultado.
//Suma
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result += rhs;
	return result;
}

//Resta
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result -= rhs;
	return result;
}

//Multiplicación
inline Vector2D operator*(const Vector2D &lhs, double rhs)
{
	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(double lhs, const Vector2D &rhs)
{
	Vector2D result(rhs);
	result *= lhs;
	return result;
}

//División
inline Vector2D operator/(const Vector2D &lhs, double rhs)
{
	Vector2D result(lhs);
	result /= rhs;
	return result;
}

inline Vector2D operator/(double lhs, const Vector2D &rhs)
{
	Vector2D result(rhs);
	result.x = lhs/result.x;
	result.y = lhs/result.y;
	return result;
}

//Funciones para vectores
//Módulo
inline double vector2DModule(const Vector2D &v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

//Normalización
inline void vector2DNormalize(Vector2D &v)
{
	double vectorMod = vector2DModule(v);

	v.x = v.x / vectorMod;
	v.y = v.y / vectorMod;

}

//Producto escalar
inline double vector2DEscalar(Vector2D &v1, Vector2D &v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

//Signo del ángulo formado con otro vector
inline int vector2DSign(Vector2D &v1, Vector2D &v2)
{
	if(v1.y * v2.x > v1.x*v2.y)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}










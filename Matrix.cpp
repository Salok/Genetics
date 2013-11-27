//Trabajando con matrices. Y puntos que transformar con las matrices.
//En un 2D un 120% más plano!
struct Point
{
	double x;
	double y;
	Point(double a = 0.0f, double b = 0.0f):x(a), y(b){}
};

class matrix2D
{
	private:
		struct _2DMat
		{
			double _11, _12, _13;
			double _21, _22, _23;
			double _31, _32, _33;

			_2DMat()
			{
				_11 = 0; _12 = 0; _13 = 0;
				_21 = 0; _22 = 0; _23 = 0;
				_31 = 0; _32 = 0; _33 = 0;
			}

		//Overloading ostream operator para poder hacer print de matrices.
			friend ostream &operator<<(ostream& os, const _2DMat &rhs)
			{
				os << "\n" << rhs._11 << " " << rhs._12 << " " << rhs._13;
				os << "\n" << rhs._21 << " " << rhs._22 << " " << rhs._23;
				os << "\n" << rhs._31 << " " << rhs._32 << " " << rhs._33;

				return os;

			}
		};

		_2DMat my_Matrix;

		//Filas por columnas. Multiplicar matrices es raro.
		inline void _2DMatMultiply(_2DMat &mult)
		{
			_2DMat mat_temp;
			//Primera fila
			mat_temp._11 = (my_Matrix._11*mult._11) + (my_Matrix._12*mult._21) + (my_Matrix._13*mult._31);
			mat_temp._12 = (my_Matrix._11*mult._12) + (my_Matrix._12*mult._22) + (my_Matrix._13*mult._32);
			mat_temp._13 = (my_Matrix._11*mult._13) + (my_Matrix._12*mult._23) + (my_Matrix._13*mult._33);

			//Segunda fila
			mat_temp._21 = (my_Matrix._21*mult._11) + (my_Matrix._22*mult._21) + (my_Matrix._23*mult._31);
			mat_temp._22 = (my_Matrix._21*mult._12) + (my_Matrix._22*mult._22) + (my_Matrix._23*mult._32);
			mat_temp._23 = (my_Matrix._21*mult._13) + (my_Matrix._22*mult._23) + (my_Matrix._23*mult._33);

			//Tercera fila
			mat_temp._31 = (my_Matrix._31*mult._11) + (my_Matrix._32*mult._21) + (my_Matrix._33*mult._31);
			mat_temp._32 = (my_Matrix._31*mult._12) + (my_Matrix._32*mult._22) + (my_Matrix._33*mult._32);
			mat_temp._33 = (my_Matrix._31*mult._13) + (my_Matrix._32*mult._23) + (my_Matrix._33*mult._33);

			my_Matrix = mat_temp;
		}

	public:
		matrix2D()
		{
			//Inicializamos la matriz como una matriz identidad.
			Identity();
		}

		//Crea una matriz identidad
		void Identity()
		{
			my_Matrix._11 = 1; my_Matrix._12 = 0; my_Matrix._13 = 0;
			my_Matrix._21 = 0; my_Matrix._22 = 1; my_Matrix._23 = 0;
			my_Matrix._31 = 0; my_Matrix._32 = 0; my_Matrix._33 = 1;
		}

		//Crea una matriz de traslación y transforma la matriz original
		void Translate(double x, double y)
		{
			_2DMat temp;

			temp._11 =  1; temp._12 = 0; temp._13 = 0;
			temp._21 =  0; temp._22 = 1; temp._23 = 0;
			temp._31 =  x; temp._32 = y; temp._33 = 1;

			_2DMatMultiply(temp);
		}

		//Crea una matriz de escalado y escala la matriz original
		void Scale(double xScale, double yScale)
		{
			_2DMat temp;

			temp._11 = xScale; temp._12 = 0;      temp._13 = 0;
			temp._21 =  0;     temp._22 = yScale; temp._23 = 0;
			temp._31 =  0;     temp._32 = 0;      temp._33 = 1;

			_2DMatMultiply(temp);
		}

		//Crea una matriz de rotacion y rota la matriz original
		void Rotate(double rotation)
		{
			_2DMat temp;
			double Cos = cos(rotation);
			double Sin = sin(rotation);

			temp._11 = Cos;  temp._12 = Sin; temp._13 = 0;
			temp._21 = -Sin; temp._22 = Cos; temp._23 = 0;
			temp._31 =  0;   temp._32 = 0;   temp._33 = 1;

			_2DMatMultiply(temp);
		}

		//Aplica una matriz de transformación a una serie de puntos
		inline void transformPoints(vector<Point> &vecPoint)
		{
			for(int i = 0; i<vecPoint.size(); i++)
			{
				double tempX = vecPoint.x * my_Matrix._11 + vecPoint.y * my_Matrix._21 + my_Matrix._31;
				double tempY = vecPoint.x * my_Matrix._12 + vecPoint.y * my_Matrix._22 + my_Matrix._32;
				vecPoint[i].x = tempX;
				vecPoint[i].y = tempY;
			}
		}
};
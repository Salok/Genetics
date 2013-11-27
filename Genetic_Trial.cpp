//Given the number 42, how can you express it as a sum, sustraction, multiplication 
//or division of the numbers 0 to 9 with only 4 operations?
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int N = 42;
int generacion = 10;

// Diez cromosomas con nueve genes (cinco digitos y cuatro operaciones)
int chromosome[10][9];
int children[10][9];
int OTP[2][9];
float fitness[10];

int gen()
{
	int x;
	//Devuelve un valor aleatorio entre 1 y 13. Del 1 al 9 para los números, del 10 al 13
	//para las operaciones. 
	x = rand() % 13 + 1;
	return x;
};

//Crea una lista inicial de cromosomas aleatorios.
void iniciador(int generacion)
{
	puts("Inicializando");
	for(int i = 0; i<generacion; i++)
	{
		for(int j=0; j<9; j++)
		{
			chromosome[i][j] = gen();
		}
	}
};

//Desencripta el cromosoma y devuelve su resultado.
float solve(int chromosome[9])
{
	float x = 0.0f;
	int j = 0;
	int k = 0;
	int count = 0;
	int digits[5];
	digits[0] = -1;
	digits[1] = -1;
	int operations[4];

	//Dividimos el gen en operaciones y digitos.
	for(int i = 0; i<9; i++)
	{
		if(chromosome[i]>9 && count == 1)
		{
			if (j<4)
			{
				operations[j] = chromosome[i];
				j++;
				count = 0;
			}
		}
		else if(chromosome[i]<=9 && count == 0)
		{
			if(k<5)
			{
				digits[k] = chromosome[i];
				k++;
				count = 1;
			}
		}
	}
	for(int i = 0; i<k; i++)
	{
		printf("Gen_Digit %i: %i \n", i, digits[i]);
	}

	for(int i = 0; i<j; i++)
	{
		printf("Gen_Op %i: %i \n", i, operations[i]);
	}
	//Por cada operacion cogemos la solución anterior y el siguiente digito
	//y operamos.
	if (digits[0] != -1 && digits[1] != -1)
	{	
		x = digits[0];
		for (int i = 0; i<k; i++)
		{

			switch (operations[i])
			{
				case 10:
					x += digits[i+1];
					break;

				case 11:
					x -= digits[i+1];
					break;

				case 12:
					x *= digits[i+1];
					break;

				case 13:
					x /= digits[i+1]; 
					break;

				default:
					break;
			}
		}
	}

	else if (digits[1] == -1)
		x = digits[0];
	else
		x = 0;
	return x;

}

//Devuelve un valor menor que uno que es mayor cuanto mejor sea el cromosoma.
float fitnessCalc(int chromosome[9])
{
	float fit;
	float solution = solve(&chromosome[0]);
	printf("Solution is: %f \n", solution);
	if(solution != N)
		fit = 1/(N-solution);
	else
		fit = 10.0f;
	printf("Fitness is: %f \n", fit);
	if(fit < 0)
		fit *= -1;
	return fit;
};



//Sobrecruzamiento cromosomico. Añadimos variabilidad a la reproducción.
void crossingOver(int childNum)
{
	int x = rand() % 9;
	int y;
	for(int i = x; i<9; i++)
	{
		y = children[2*childNum][i];
		children[2*childNum][i] = children[2*childNum+1][i];
		children[2*childNum+1][i] = y;
	}

}

//Añadimos alguna mutación para darle más gracia a todo esto.
void mutate(int chromNum)
{
	int x = rand() % 9;
	children[chromNum][x] = gen();
}

//Función que escoge un cromosoma al azar (biased by fitness)
int russianRoulette()
{	

	for(int i = 0; i<9; i++)
	{
		if(fitness[i] != 0.0f)
		{
			if(rand() % 1000 <= floor(fitness[i]*1000))
				return i;
		}
	}
}

//Survival of the fittest. Se elige a los cromosomas a reproducirse según su resultado.
//Producimos N hijos donde N sea el número de padres.

int naturalSelection(int generacion)
{
	int x, y;
	int chrom;
	float solved = 0.0f;
	for(int i = 0; i<generacion; i++)
	{
		printf("chromosome %i: \n", i);
		solved = fitnessCalc(chromosome[i]);
		if(solved < 10.0f)
			fitness[i] = solved;
		else
			return i;
	}

	for (int i = 0; i<(generacion/2); i++)
	{
		x = rand() % 10;
		for(int j = 0; j<2; j++)
		{	
			chrom = russianRoulette();
			for(int k = 0; k<9; k++)
			{
				children[2*i+j][k] = chromosome[chrom][k];
			}
		}
		if (x < 7)
			crossingOver(i);
		y = rand() % 100;
		if(y == 99)
			mutate(2*i);
		else if (y == 1)
			mutate(2*i+1);
	}
	for(int i = 0; i<10; i++)
	{
		for(int j = 0; j<9; j++)
			chromosome[i][j] = children[i][j];
	}
	puts("Next generation \n ");
	return -1;
}

int main()
{	
	srand(time(NULL));
	int gotcha = -1;
	iniciador(10);
	while (gotcha == -1)
	{
	gotcha = naturalSelection(generacion);
	}

	for(int i = 0; i<9; i++)
	{
		printf("Gen %i: %i \n", i, chromosome[gotcha][i]);
	}
	return 0;
}
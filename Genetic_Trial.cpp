//Given the number 42, how can you express it as a sum, sustraction, multiplication 
//or division of the numbers 0 to 9 with only 4 operations?
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
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
	//Devuelve un valor aleatorio entre 0 y 13. Del 0 al 9 para los números, del 10 al 13
	//para las operaciones. 
	x = rand() % 14 ;
	return x;
};

//Crea una lista inicial de cromosomas aleatorios.
int iniciador(int generacion)
{
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
	int solution;
	int x = 0;
	for(int i = 0; i<9; i++)
	{
		//Si el gen es un número busca el suguiente operador y el siguiente número y opera.
		//Si falla el resultado será cero.
		if(chromosome[i] <= 9)
		{
			for(int j = i; j<(9-i); j++)
			{
				if (chromosome[j] > 9)
				{
					for(int k = j; k<(9-i-j); k++)
					{
						if (k <= 9)
						{
							switch(chromosome[j])
							{
								case 10:
									x = chromosome[i]+chromosome[k];
									break;

								case 11:
									x = chromosome[i] - chromosome[k];
									break;

								case 12:
									x = chromosome[i]*chromosome[k];
									break;

								case 13:
									x = chromosome[i]/chromosome[k];
									break;
							}
							i = k+1;
							break;
						}
					}
					break;
				}
			}
		}
		solution += x;
	}

	return solution;
}

//Devuelve un valor menor que uno que es mayor cuanto mejor sea el cromosoma.
float fitnessCalc(int chromosome[9])
{
	float fit;
	float solution = solve(&chromosome[9]);
	if(solution != N)
		fit = 1/(N-solution);
	else
		fit = 1.0f;
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
	int x;
	int chromNum;
	for (int i = 0; i<generacion; i++)
	{
		x += floor(100*fitness[i]);
	}

	x = rand() % x;

	for(int i = 0; i<generacion; i++)
	{
		x -= floor(100*fitness[i]);
		if(x<0)
		{
			chromNum = i;
			break;
		}
	}
	return chromNum;
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
		solved = fitnessCalc(chromosome[i]);
		if(solved != 1.0f)
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
		y = rand() % 1000;
		if(y==0)
			mutate(2*i);
		else if (y == 1)
			mutate(2*i+1);
	}
	return -1;
}

int main()
{	
	int gotcha = -1;

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
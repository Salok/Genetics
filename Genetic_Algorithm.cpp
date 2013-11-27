//Genetic Algorithm classes and methods for Neural Network.

//Clase con el genoma de cada red neuronal. Incluye el peso de sus 
// conexiones y su capacidad de supervivencia (fitness).
class Genome
{
	public:
		//Vector con los pesos de las conexiones
		vector<double> Weights;
		//Capacidad de superviviencia
		double Fitness;

		//Constructor sin argumentos
		Genome():Fitness(0){}
		//Constructor
		Genome(vector<double> weightVec, double fit=0)
		{
			Weights = weightVec;
			Fitness = fit;
		}

		//Overload de < para que pueda actuar con genomas.
		friend bool operator< (const Genome& lhs, const Genome& rhs)
		{
			return (lhs.Fitness < rhs.Fitness);
		} 

};

//Clase del algoritmo genético. Guarda todos los genomas y los hace
//luchar a muerte y reproducirse.

class geneticAlgorithm
{
	private:
		//Vector con todos los genomas.
		vector<Genome> population;
		//Tamaño de la población
		int populationSize;
		//Tamaño de los genomas
		int genomeLength;
		//Capacidad de la población
		double totalFitness;
		//Mayor capacidad en la población
		double bestFitness;
		//Capacidad media
		double averageFitness;
		//Peor cpacidad de la población
		double worstFitness;
		//Mejor genoma
		int fittestGenome;
		//Peor genoma
		int fattestGenome;
		//Probabilidad de mutación
		double mutationRate;
		//Probabilidad de sobrecruzamiento
		double crossoverRate;
		//Contador de generaciones
		int generationCount;

		//Constructor. Crea los genomas y sus respectivas redes neurales.
		geneticAlgorithm(int popsize, double	MutRat = Params::mutationRate, double CrossRat = Params::crossoverRate, int genLength)
		{
			populationSize = popsize;
			mutationRate = MutRat;
			crossoverRate = CrossRat;
			genomeLength = genLength;
			Reset();

			for(int i = 0; i<populationSize; i++)
			{
				population.push_back(Genome());
				for(int j = 0; j<genomeLength; j++)
				{
					population[i].Weights[j].randFloat();
				}
			}
			genomeLength = population[0].Weights.size();
		}

		//Devuelve un genoma. Las posibilidades de cada uno son mayores según su capacidad.
		Genome genomeRoulette()
		{
			//Un número entre el 0 y la capacidad total.
			double roulette = (randFloat(0.0f, totalFitness));
			double init = 0;
			//Genoma elegido
			Genome theChosenOne;

			//Iteramos por la lista de genomas. Cuando sobrepasamos el numero aleatorio paramos.

			for(int i = 0; i<populationSize; i++)
			{
				init += population[i].Fitness;
				if(init >= roulette)
				{
					theChosenOne = population[i];
					break;
				}
			}
			return theChosenOne;
		}

		//Función Crossover. Coge dos genomas padres y devuelve dos hijos
		void Crossover(const vector<double> &mum, const vector<double> &dad, vector<double> &baby1, vector<double> &baby2)
 		{
 			int crossPoint;
 			//Si los padres son los mismos o no hay sobrecruzamiento los devolvemos.
 			if(randFloat(0.0f, 1.0f) > crossoverRate || (mum == dad))
 			{
 				baby1 = mum;
 				baby2 = dad;

 				return;
 			}

 			crossPoint = randInt(dad.size());

 			for(int i = 0; i<crossPoint; i++)
 			{
 				baby1.push_back(mum[i]);
 				baby2.push_back(dad[i]);
 			}

 			for(int i = crossPoint; i<dad.size(); i++)
 			{
 				baby1.push_back(dad[i]);
 				baby2.push_back(mum[i]);
 			}
 		}

 		//Cogemos un genoma e iteramos por sus genes. Si en alguno random es menos que la tasa de mutación cambiamos el gen.
 		void Mutate(vector<double> &baby)
 		{
 			for(int i = 0; i<baby.size(); i++)
 			{
 				if(randFloat(0.0f, 1.0f) < mutationRate)
 				{
 					baby[i] += randFloat() * Params::maxPerturbation;
 				}
 			}
 		}

 		//Elitismo para que el azar no eche para atras a nuestra población.
 		//Cogemos N veces los M genomas mejor preparados y los añadimos a la lista.
 		//La población que introducimos debe estar ordenada según su capacidad.
 		void eliteCloning(int NBest, const int Copies, vector<Genome> &pop)
 		{
 			while(NBest--)
 			{
 				for(int i = 0; i<Copies; i++)
 				{
 					pop.push_back(population[populationSize-1-NBest]);
 				}
 			}
 		}

 		//Calcula la capaciad total, la mejor, la peor y la media.
 		void calculateSocialFitness()
 		{
 			totalFitness = 0;
 			for(int i = 0; i<populationSize; i++)
 			{
 				if(population[i].Fitness > bestFitness)
 				{
 					 bestFitness = population[i].Fitness;
 					 fittestGenome = i;
 				}

 				if(population[i].Fitness < worstFitness)
 				{
 					worstFitness = population[i].Fitness;
 					fattestGenome = i;
 				}

 				totalFitness += population[i].Fitness;
 			}
 			averageFitness = totalFitness/populationSize;
 		}

 		//Reinicia algunos factores del algoritmo para empezar una nueva generación.
 		void Reset()
 		{
 			totalFitness = 0;
			generationCount = 0;
			fittestGenome = 0;
			bestFitness = 0;
			worstFitness = 999999;
			averageFitness = 0;
 		}

 		//Coge una generación de genomas y los hace pasar por un ciclo de reproducción. Devuelve la próxima generación.
 		vector<Genome> naturalSelection(vector<Genome> &old_pop)
 		{
 			//Asignamos la población para poder utilizar las funciones de la clase.
 			population = old_pop;

 			//Reseteamos la clase para empezar la nueva generación.
 			Reset();

 			//Ordenamos la nueva población para el elitismo
 			sort(population.begin(), population.end());

 			//Calculamos la capacidad de la población
 			calculateSocialFitness();

 			//Creamos un vector donde guardar la nueva generación.
 			vector<Genome> newPop;

 			//Añadimos las copias de los mejores habitantes de la población anterior. Comprobamos que sea par para que funcione todo bien.
 			if(!(Params::NBest*Params::CloneNum % 2))
 				eliteCloning(Params::NBest, Params::CloneNum, newPop);

 			//Empezamos a reproducir los genomas antiguos
 			while(newPop.size() < populationSize)
 			{
 				//Elegimos a los padres.
 				Genome mum = genomeRoulette();
 				Genome dad = genomeRoulette();

 				//Creamos espacio para los hijos
 				vector<double> baby1, baby2;

 				//Creamos a los hijos...
 				Crossover(mum.Weights, dad.Weights, baby1, baby2);
 				//...los mutamos...
 				Mutate(baby1);
 				Mutate(baby2);
				//...y luego entran en la población
 				newPop.push_back(Genome(baby1, 0));
 				newPop.push_back(Genome(baby2, 0));
 			}

 			population = newPop;
 			return population;
 		}
};

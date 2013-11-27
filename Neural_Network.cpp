#include <vector.h>
#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Función random para dar el peso a los inputs de las neuronas. Default: Random entre -1 y 1.
double randFloat(double min=-1.0, double max=1.0)
{
	double x;
	x = min + static_cast<double> (rand()/(static_cast<double> (RAND_MAX/(max-min))));
	return x;
}

//Funcion random que devuelve integers. Default entre 0 y max.
int randInt(int max, int min = 0)
{
	int x; 
	x = min + static_cast<int>(rand() % (max-min+1));
	return x;
}

//Clase con los parametros para el programa.
class Params
{
public:
	//Para la red neural
	static int inputs;
	static int outputs;
	static int layerNum;
	static int neurons;
	//Para la función sigmoidea
	static double response;
	
	/*------------------------------------------*/

	//Para el algoritmo genético
	static double crossoverRate;
	static double mutationRate;

	//Para controlar las mutaciones
	static double maxPerturbation;

	//Para el elitismo
	static int NBest;
	static int CloneNum;

};

//Clase Neurona. Necesita un número de inputs y crea al azar un peso para cada uno. Guarda eso en un vector.
class Neuron
{
	public:
		int inputs;
		vector<double> inputWeight;
		Neuron(int inputNum)
		{
			inputs = inputNum+1;
			for(int i = 0; i<inputs; i++)
			{
				inputWeight.push_back(randFloat());
			}
	}  
};

//Clase de la capa de neuronas. Necesita un número de neuronas y un número de inputs para esas neuronas. Llama n veces a la clase neurona y las guarda
// en un vector de neuronas.
class neuronLayer 
{
	public:
		int neurons;
		vector<Neuron> neuronsInLayer;
		neuronLayer(int neuronNum, int inputNum)
		{
			neurons = neuronNum;
			for(int i = 0; i<neuronNum; i++)
			{
				neuronsInLayer.push_back(Neuron(inputNum));
			}
		}
};

//Clase network. Crea la red neuronal llamando a neuronLayer n veces. Además crea una capa de inputs y otra de output.

class neuralNet
{
	private:
		//Inputs a la red
		int inputs;
		//Outputs de la red
		int outputs;
		//Capas de neuronas entre la de inputs y la de outputs
		int hiddenLayers;
		//Neuronas en cada capa intermedia
		int neuronsPerLayer;
		//Vector con todas las capas de la red
		vector<neuronLayer> brain;

	public:
		//Constructor de la clase
		neuralNet(int inputNum=Params::inputs, int outputNum=Params::outputs, int layerNum=Params::layerNum, int neuronNum=Params::neurons)
		{
			hiddenLayers = layerNum;
			inputs = inputNum;
			outputs = outputNum;
			neuronsPerLayer = neuronNum;
			brain.push_back(neuronLayer(neuronNum, inputNum));
			for(int i = 0; i<layerNum; i++)
			{
				brain.push_back(neuronLayer(neuronNum, neuronNum));
			}
			brain.push_back(neuronLayer(outputNum, neuronNum));
		}

		//Devuelve un vector con los pesos de todos los inputs de las neuronas de la red.
		vector<double> getWeights() const
		{
			vector<double> weights;
			int brainSize = brain.size();
			int layerSize;
			int neuronSize;
			for(int i = 0; i<brainSize; i++)
			{
				layerSize = brain[i].neurons;
				for(int j = 0; j<layerSize; j++)
				{
					neuronSize = brain[i].neuronsInLayer[j].inputs;
						for(int k = 0; k<neuronSize; k++)
						{
							weights.push_back(brain[i].neuronsInLayer[j].inputWeight[k]);
						}
				}
			}
			return weights;
		}

		//Devuelve el número de conexiones de la red.
		int connectionNum() const
		{
			int connections = 0;
			int brainSize = brain.size();
			int layerSize;
			int neuronSize;
			for(int i = 0; i<brainSize; i++)
			{
				layerSize = brain[i].neurons;
				for(int j = 0; j<layerSize; j++)
				{
					connections += brain[i].neuronsInLayer[j].inputs;
				}

			}
			return connections;
		}


		//Reemplaza el peso de cada conexión con uno nuevo.
		void putWeights(vector<double> &weights)
		{
			int brainSize = brain.size();
			int layerSize;
			int neuronSize;
			int l = 0;
			for(int i = 0; i<brainSize; i++)
			{
				layerSize = brain[i].neurons;
				for(int j = 0; j<layerSize; j++)
				{
					neuronSize = brain[i].neuronsInLayer[j].inputs;
						for(int k = 0; k<neuronSize; k++)
						{
							brain[i].neuronsInLayer[j].inputWeight[k] = weights[l];
							l ++;
						}
				}
			}
		}


		//Función sigmoidea que devuelve un valor entre cero y uno dependiendo de la activación. Al reducir la respuesta se acerca a una función
		//con un valor de activación inicial que devuelve cero antes y uno después de ese valor.
		inline double sigmoidFunction(double activation, double response)
		{
			return 1/(1+exp(-activation/response));

		}

		//Coge un vector de inputs y los pasa por todas las neuronas de la red para obtener un resultado.
		vector<double> Update(vector<double> &inputArray)
		{
			//Vector resultado con los outputs.
			vector<double> outputArray;

			//Primero miramos si tenemos el número correcto de inputs.
			if(inputArray.size() != inputs)
			{
				return outputArray;
			}

			//Iteramos por nuestras neuronas, capa por capa.
			int brainSize = brain.size();
			int layerSize;
			int neuronSize;
			double neuronOutput;

			//Para cada capa...
			for(int i = 0; i<brainSize; i++)
			{
				layerSize = brain[i].neurons;

				// A partir de la segunda capa cogemos los outputs de la
				//capa anterior como inputs.
				if(i>0)
					inputArray = outputArray;
				outputArray.clear();

				//Para cada neurona...
				for(int j = 0; j<layerSize; j++)
				{
					neuronSize = brain[i].neuronsInLayer[j].inputs;

					//Para cada conexion de la neurona... (excepto el bias)
						for(int k = 0; k<inputArray.size(); k++)
						{
							neuronOutput += brain[i].neuronsInLayer[j].inputWeight[k]*inputArray[k];
						}

					//Añadimos el bias
					neuronOutput -= brain[i].neuronsInLayer[j].inputWeight[inputArray.size()];
					//Pasamos el resultado por la función sigmoidea y lo metemos en el output.
					outputArray.push_back(sigmoidFunction(neuronOutput, 1.0));
				}
			}
			return outputArray;
		}
};

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
		geneticAlgorithm(int popsize, double	MutRat = Params::mutationRate, double CrossRat = Params::crossoverRate)
		{
			populationSize = popsize;
			mutationRate = MutRat;
			crossoverRate = CrossRat;
			Reset();

			for(int i = 0; i<populationSize; i++)
			{
				population.push_back(Genome(neuralNet().getWeights()));
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


int main()
{
	srand(time(NULL));
	neuralNet brain_1(1, 2, 1, 2);

	int connections = brain_1.connectionNum();
	cout<<endl<<connections;
	vector<double> x = brain_1.getWeights();
	vector<double> y;
	y.push_back(-1.0f);

	for(int i = 0; i<x.size(); i++)
	{
		cout<<endl<<x[i];
		x[i] = 1;
	}

	brain_1.putWeights(x);

	for(int i = 0; i<x.size(); i++)
	{
		cout<<endl<<brain_1.getWeights()[i];
	}

	x = brain_1.Update(y);

	for(int i = 0; i<x.size(); i++)
	{
		cout<<endl<<x[i];
		x[i] = 1;
	}


	return 0;
}





















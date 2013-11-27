#include <vector.h>
#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


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





















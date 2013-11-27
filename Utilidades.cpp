//Funciones y clases necesarias para Neural_Network (No genéticas ni neuronales)
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

	/*-------------------------------------------*/

};

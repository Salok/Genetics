/* Ejemplo de aplicación de redes neurales. Creamos unos vehículos que recogen minas. Cada mina sube un punto de Fitness. Reciben dos inputs 
bidimensionales: Un vector con la distancia a la mina más cercana y el vector de rotación del vehículo. Su red neuronal les devuelve dos outputs:
una fuerza para aplicar en su eje izquierdo y otra en el derecho.*/

class mineSweeper
{
	private:
		//Cerebro del vehículo
		neuralNet	msBrain;
		//Posición del vehículo
		Vector2D	msPosition;
		//

};
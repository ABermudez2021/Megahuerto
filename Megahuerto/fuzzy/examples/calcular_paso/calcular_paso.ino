/*
 * Programa creado por Elmer Meneses
 * Ingeniero Mecatrónico, universidad de Pamplona
 * Programa para calcular el valor ideal del parámetro "Paso"  que el tamaño de vector de salida no deberia superar el valor de 100
 */
#include <fuzzy.h> // Incluimos la librería
fuzzy controlfuzzy; // Creamos un objeto de tipo fuzzy.
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////Definimos conjunto para variable de entrada
float USAL[] = {-50,50}; // Definimos el universo de discurso (rango)de la variable salida
int paso = 1; //Se define la cantidad mínima de variación para los numeros del conjunto de salida
const int tam = controlfuzzy.calc_size(USAL,paso); // Calculamos el tamaño que tendrá el vector del conjunto de salida con universo salida y "paso"
void setup() { //Condiguracion
Serial.begin(9600);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  /*
    Se imprime el valor de tamaño para ver la cantidad de datos en el conjunto de salida, este valor no debe superar los 100
   */
  Serial.println(tam); // Se imprime el tamaño del vector, si supera los 100 pruebe a modificar el parámetro "paso".
	delay(50);
}

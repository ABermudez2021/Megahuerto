/*
 * Programa creado por Elmer Meneses
 * Ingeniero Mecatrónico, universidad de Pamplona
 * Programa para un control fuzzy con una sola entrada
 */
#include <fuzzy.h> // Incluimos la librería
fuzzy controlfuzzy; // Creamos un objeto de tipo fuzzy.
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////Definimos conjunto para variable de entrada
float USAL[] = {-50,50}; // Definimos el universo de discurso (rango)de la variable salida
float UIN[] = {-1500,1500};// Definimos universo de discurso de la variable de entrada
float ENP[] = {-1500,-666,0,'T'}; //Definimos los conjuntos difusos, colocando 'T' si es triangular, o 'R' si es trapezoidal
float EC[] = {-5,0,5,'T'};// Conjuntos difusos de entrada
float EPP[] = {6,666,1500,'T'};
///////////////////////////////////////////////////////
////////////////////////////////////////////////////// Definimos conjuntos variable de salida
float DT[] = {-40,-30,-20,-10,'R'}; //Conjunto difuso trapezoidal
float ZE[] = {-10,0,10,'T'};  //Conjunto difuso triangular
float AT[] = {10,20,30,40,'R'};// Conjunto difuso trapezoidal
float paso = 2; //Se define la cantidad mínima de variación para los numeros del conjunto de salida
float setpoint =1000,rpm=0,error;
const int tam = controlfuzzy.calc_size(USAL,paso); // Calculamos el tamaño que tendrá el vector del conjunto de salida con universo salida y "paso"
void setup() { //Configuración
Serial.begin(9600);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  float B[tam]; //Creamos el vector que guardará los numeros del conjunto de salida
  controlfuzzy.inicio(B,tam); //Inicializamos el objeto fuzzy
  error=setpoint-rpm;//Calculamos la variable error
  /*
   * Se empieza con la inferencia, y para ello las reglas. Para llamar a una regla con una entrada se utiliza la función "regla_simple"
   * la cual recibe como parametros (Conjunto entrada,Universo de entrada,variable a evaluar,conjunto salida,universo salida,vector salida,tamaño
   * vector salida)
   */
  controlfuzzy.regla_simple(ENP,UIN,error,DT,USAL,B,tam);//Llamamos a la función para aplicar la regla correspondiente
  controlfuzzy.regla_simple(EC,UIN,error,ZE,USAL,B,tam);
  controlfuzzy.regla_simple(EPP,UIN,error,AT,USAL,B,tam);
  float res = controlfuzzy.defusi(B,USAL,tam); //Se llama a la función para defusificar el conjunto de salida y obtener el resultado del sistema fuzzy
  rpm = rpm+res; // Se asigna ese resultado a la variable control.
  Serial.println(rpm); //Se imprime el valor de rpm que debería converger al valor de setpoint
	delay(50);
  /*
   * En este ejemplo el valor de rpm deberia seguir el valor definido en setpoint
   */
}

/*
 * Programa creado por Elmer Meneses
 * Ingeniero Mecatrónico, universidad de Pamplona
 * Programa para un control fuzzy con dos entradas
 */
#include <fuzzy.h> // Incluimos la librería
fuzzy controlfuzzy; // Creamos un objeto de tipo fuzzy.
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////Definimos conjunto para variable de entrada
float USAL[] = {-120,120}; // Definimos el universo de discurso (rango)de la variable salida
float UIN[] = {-1500,1500};// Definimos universo de discurso de la variable de entrada
float UIN2[] = {-50,50};// Definimos universo de discurso de la segunda variable de entrada
float ENP[] = {-1500,-666,0,'T'}; //Definimos los conjuntos difusos, colocando 'T' si es triangular, o 'R' si es trapezoidal
float EC[] = {-5,0,5,'T'};// Conjuntos difusos de entrada
float EPP[] = {6,666,1500,'T'};
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////Definimos conjunto para segunda variable de entrada
float DNP[] = {-50,-50,-30,0,'R'}; //Definimos los conjuntos difusos, colocando 'T' si es triangular, o 'R' si es trapezoidal
float DC[] = {-5,0,5,'T'};// Conjuntos difusos de entrada
float DPP[] = {0,30,50,50,'R'};
///////////////////////////////////////////////////////
////////////////////////////////////////////////////// Definimos conjuntos variable de salida
float DT[] = {-40,-30,-20,-10,'R'}; //Conjunto difuso trapezoidal
float ZE[] = {-10,0,10,'T'};  //Conjunto difuso triangular
float AT[] = {10,20,30,40,'R'};// Conjunto difuso trapezoidal
float paso = 2; //Se define la cantidad mínima de variación para los numeros del conjunto de salida
float error,derror;
const int tam = controlfuzzy.calc_size(USAL,paso); // Calculamos el tamaño que tendrá el vector del conjunto de salida con universo salida y "paso"
void setup() { //Configuración
Serial.begin(9600);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  float B[tam]; //Creamos el vector que guardará los numeros del conjunto de salida
  controlfuzzy.inicio(B,tam); //Inicializamos el objeto fuzzy enviandole el vector salida, y su tamaño
  error=500;
  derror=30;
  /*
   * Se empieza con la inferencia, y para ello las reglas. Para llamar a una regla con dos entradas se utiliza la función "regla_compuesta2"
   * la cual recibe como parametros (Conjunto entrada1,conjunto entrada2,Universo de entrada1,Universo de entrada2,variable a evaluar1,variable evaluar2,
   * conjunto salida,universo salida,vector salida,tamaño vector salida)
   */
  controlfuzzy.regla_compuesta2(ENP,DNP,UIN,UIN2,error,derror,DT,USAL,B,tam);//Llamamos a la función para aplicar la regla correspondiente
  controlfuzzy.regla_compuesta2(EC,DC,UIN,UIN2,error,derror,ZE,USAL,B,tam);//Llamamos a la función para aplicar la regla correspondiente
  controlfuzzy.regla_compuesta2(EPP,DPP,UIN,UIN2,error,derror,AT,USAL,B,tam);//Llamamos a la función para aplicar la regla correspondiente
  /*
   * Para llamar a la funcion defusi se tiene que enviar los parámetros (conjunto salida,1 para tipo centroide,universo salida,tamaño vector salida)
   */
  float res = controlfuzzy.defusi(B,USAL,tam); //Se llama a la función para defusificar el conjunto de salida y obtener el resultado del sistema fuzzy
  Serial.println(res); //Se imprime el valor de salida modelo fuzzy
	delay(50);
}

/*
***********************Librería creada por Elmer Meneses *******************************
***********************Ingeniero Mecatrónico Universidad de Pamplona********************
***********************  9 de Agosto de 2019             *******************************
*/
#ifndef fuzzy_h
#define fuzzy_h
#include <arduino.h>
class fuzzy
{
public:
	int calc_size(float vec[], float paso);
	float triangular(float x[],float params[],float date);
	float trapezoidal(float x[],float params[],float date);
	void inferencia_mamdani2(float B[],float c,float stru[],float U_VOL[],int tam);
	float defusi(float conju[],float U_VOL[],int tam);
	float absoluto(float dat);
	void regla_compuesta2(float in1[],float in2[],float U1[],float U2[],float c1,float c2,float V[],float UV[],float B[],int tam);
	void regla_compuesta3(float in1[],float in2[],float in3[],float U1[],float U2[],float U3[],float c1,float c2,float c3, float V[], float UV[], float B[], int tam);
	void regla_simple(float in[],float Uin[],float err,float V[], float UV[], float B[],int tam);
	void inicio(float B[],int tama);
	void borrar(float B[],int tama);
};
#endif
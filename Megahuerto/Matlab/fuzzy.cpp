/*
***********************Librería creada por Elmer Meneses *******************************
***********************Ingeniero Mecatrónico Universidad de Pamplona********************
***********************  9 de Agosto de 2019             *******************************
*/
#include <Arduino.h>
#include "fuzzy.h"
void fuzzy::inicio(float B[],int tama){
  for(int i=0; i<tama; i++){
      B[i] = 0;
  }
}
void fuzzy::borrar(float B[],int tama){
  for(int i=0; i<tama; i++){
      B[i] = 0;
  }
}
int fuzzy::calc_size(float vec[], float paso){
  int res = 0;
  res = ((absoluto(vec[0]))+absoluto((vec[1])))/paso;
  return res;
}
void fuzzy::regla_simple(float in[],float Uin[],float err,float V[], float UV[], float B[], int tam){
  int t=0,date=0;
  float r=0.0;
  while(date==0){
      if(in[t]=='T' || in[t]=='t' || in[t]=='R' || in[t]=='r'){
        date=1;
        t=t-1;
        }
      t=t+1;
    }
    if(t ==3){
      r = triangular(Uin,in,err);
    }
    else if(t ==4){
      r = trapezoidal(Uin,in,err);
    }
    inferencia_mamdani2(B,r,V,UV,tam); // Se Hace la primera regla
}
void fuzzy::regla_compuesta2(float in1[],float in2[],float U1[],float U2[],float c1,float c2, float V[], float UV[], float B[], int tam){
  int t1=0,date = 0,t2 =0,date1=0;
  float r1 = 0, r2 = 0,res =0;
  while(date ==0){
    while(date1==0){
      if(in2[t2]=='T' || in2[t2]=='t' || in2[t2]=='R' || in2[t2]=='r'){
        date1=1;
        t2=t2-1;
        }
      t2=t2+1;
    }
    if(in1[t1]=='T' || in1[t1]=='t' || in1[t1]=='R' || in1[t1]=='r'){
        date=1;
        t1=t1-1;
        }
      t1=t1+1;//aqui defino el tamaño de in1 e in2 
  }
  if(t1==3){
    r1 = triangular(U1,in1,c1);
  }
    else if(t1==4){
      r1 = trapezoidal(U1,in1,c1);
    }
   if(t2==3){
    r2 = triangular(U2,in2,c2);
  }
    else if(t2 ==4){
      r2 = trapezoidal(U2,in2,c2);
    }
res = min(r1,r2);
inferencia_mamdani2(B,res,V,UV,tam);
}
void fuzzy::regla_compuesta3(float in1[],float in2[],float in3[],float U1[],float U2[],float U3[],float c1,float c2,float c3, float V[], float UV[], float B[], int tam){
  int t1=0,date = 0,t2 =0,t3 =0,date1=0,date2 =0;
  float r1 = 0, r2 = 0,r3 =0,res =0;
  while(date ==0){
    while(date1==0){
      while(date2==0){
      if(in3[t3]=='T' || in3[t3]=='t' || in3[t2]=='R' || in3[t3]=='r'){
        date2=1;
        t3=t3-1;
        }
      t3=t3+1; //Aqui defino el tamaño de in3
      }
    if(in2[t2]=='T' || in2[t2]=='t' || in2[t2]=='R' || in2[t2]=='r'){
        date1=1;
        t2=t2-1;
        }
      t2=t2+1;//aqui defino el tamaño de in2
    }
    if(in1[t1]=='T' || in1[t1]=='t' || in1[t1]=='R' || in1[t1]=='r'){
        date=1;
        t1=t1-1;
        }
      t1=t1+1;//aqui defino el tamaño de in1 
  }////////////////////////////////Aqui defino el tamaño de los vectores ingresados
  if(t1==3){
    r1 = triangular(U1,in1,c1);
  }
    else if(t1==4){
      r1 = trapezoidal(U1,in1,c1);
    }
  if(t2==3){
    r2 = triangular(U2,in2,c2);
  }
    else if(t2 ==4){
      r2 = trapezoidal(U2,in2,c2);
    }
  if(t3==3){
    r3 = triangular(U3,in3,c3);
  }
    else if(t3 ==4){
      r3 = trapezoidal(U3,in3,c3);
    }
res = min(r1,r2);
res = min(r3,res);
inferencia_mamdani2(B,res,V,UV,tam);
}
float fuzzy::absoluto(float dat){// Función utilizada para calcular el absoluto de un numero 
    float res;
    if(dat <0){
      res = dat*(-1);
    }
    else{
      res = dat;
    }
    return res;
}
float fuzzy::triangular(float x[],float params[],float date){/////////////////////////////////////////////////////////////////////////////
  float y;
  if(date < params[0]){ // Se rellena con ceros a la izquierda
    y = 0;
  }
  if(date > params[2]){ // Se rellena con ceros a la derecha
    y = 0;
  }
  if (params[0]<=date && date<= params[1]){ // Primera Pendiente
    float p = 1/(params[1]-params[0]);
    y = (p*(date-params[1])+1); 
  }
  if (params[1]<=date && date<= params[2]){ // Segunda Pendiente
    float p = -1/(params[2]-params[1]);
    y = (p*(date-params[1])+1);
  }
  if(params[0] <x[0] || params[2] >x[1]){
    y = 0;
  }
  if((date == params[0]&& date ==params[1])|| (date == params[1]&& date ==params[2])){
    y = 1;
  }
  return y;
}//////////////////////////////////////////////////////////////////////////////////////////////////////
float fuzzy::trapezoidal(float x[],float params[],float date){
  float y;
  if(date < params[0]){ // Se rellena con ceros a la izquierda
    y = 0;
  }
  if(date > params[2]){ // Se rellena con ceros a la derecha
    y = 0;
  }
  if (params[0]<=date && date<= params[1]){ // Primera Pendiente
    float p = 1/(params[1]-params[0]);
    y = (p*(date-params[1])+1); 
  }
  if(params[1]<date && date<= params[2]){ // Región Constante
    y = 1;
  }
  if (params[2]<=date && date<= params[3]){ // Segunda Pendiente
    float p = -1/(params[3]-params[2]);
    y = (p*(date-params[2])+1);
  }
  if(params[0] <x[0] || params[2] >x[1]){
    y = 0;
  }
  if((date == params[2]&& date ==params[3])|| (date == params[0]&& date ==params[1])|| (date == params[1]&& date ==params[2])){
    y = 1;
  }
  return y;
}/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void fuzzy::inferencia_mamdani2(float B[],float c,float stru[],float U_VOL[],int tama){ //Hacemos inferencia mamdani
  //float j=-12; //Inicio del universo de discurso de voltaje
  int tipo,t=0,date = 0;
  while(date ==0){
    if(stru[t]=='T' || stru[t]=='t' || stru[t]=='R' || stru[t]=='r'){
      date=1;
      t=t-1;
    }
    t=t+1;
  }
  float paso,j=U_VOL[0];
  paso = (absoluto(U_VOL[0])+absoluto(U_VOL[1]))/float(tama);
  //Serial.println(paso,6);
  if(t ==3)
  {
    tipo = 2; //Función Triangular
  }
  else if(t ==4)
  {
    tipo =1; //Función tipo trapezoidal
  }
  float v[tama]; // Tamaño del vector del universo de discurso de voltaje
  for(int i=0; i <tama;i++){ //Aqui genero vector v
    v[i] = j;
    j = j+paso; 
  }
  if(tipo ==1){
    for(int i =0;i<tama;i++){
      if(v[i]<stru[0] || v[i]==stru[0]){
        float y=0;
        if(y>B[i]){
          B[i]=y;
        }
      }
      if(stru[0]<= v[i] && v[i]<=stru[1]){ //Primera Pendiente
        float p= 1/(stru[1]-stru[0]);
        float y = (p*(v[i]-stru[1]))+1;
        if(y>=c){
          y=c;
        }
        if(y>B[i]){
          B[i]=y;
        }
      }
      if(stru[1]<=v[i] && v[i]<=stru[2]){ //zona constante
        float y=c;
        if(y>B[i]){
          B[i]=y;
        }
      }
      if(stru[2]<=v[i] && v[i]<=stru[3]){ //Segunda pendiente
        float p= -1/(stru[3]-stru[2]);
        float y= (p*(v[i]-stru[2]))+1;
        if(y>=c){ //Cortamos a la altura de c1
          y=c;
        }
        if(y>B[i]){
          B[i]=y;
        }
      }
      if(v[i]==stru[2] && v[i]==stru[3]){ //ultimos puntos iguales
        float y=c;
        if(y>B[i]){
          B[i]=y;
        }
      }
      if(v[i]>stru[3]){
        float y=0;
        if(y>B[i]){
          B[i]=0;
        }
      }
    }///////////////Se acaba el primer for
  }
  if(tipo ==2){
    for(int i=0; i<tama; i++){
        if(stru[0]<= v[i] && v[i]<=stru[1]){ //Primera Pendiente
        float p= 1/(stru[1]-stru[0]);
        float y= (p*(v[i]-stru[1]))+1;
        if(y >=c){ //Esto es para hacer el recorte a la altura del valor de membresia
          y=c;
        }
        if(y >B[i]){ //Esto es para sacar el máximo, unir los conjuntos
            B[i]=y;
            //Serial.println(i);
        }
        }
        if(stru[1]<=v[i] && v[i]<=stru[2]){ //Segunda pendiente
        float p= -1/(stru[2]-stru[1]);
        float y = (p*(v[i]-stru[1]))+1;
        if(y>=c){ //Hacemos el corte a la altura del valor de membresia
          y=c;
        }
        if(y>B[i]){ //Comparamos los valores de membresia
          B[i]=y;
        }
        }
        if(v[i] ==stru[1] && v[i]==stru[2]){ //Cuando los valores 2 y 3 son iguales
          float y=0; ///////ESTO ESTA SUJETO A CAMBIOS
        }
      }
  }
}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float fuzzy::defusi(float conju[],float U_VOL[],int tama){
  float j=U_VOL[0],paso;
  paso = (absoluto(U_VOL[0])+absoluto(U_VOL[1]))/tama;
  float x[tama];
  float y0,area=0,res1=0;
  for(int i=0; i <tama;i++){
    x[i] = j;
    j = j+paso;
    area= area+conju[i];
    res1= res1+(conju[i]*x[i]);
    conju[i]=0; 
  }
  if(area ==0){
    y0 = 0;
  }
  else{
  y0 = res1/area;
  }
  return y0;
}////////////////////////////////////////////////////

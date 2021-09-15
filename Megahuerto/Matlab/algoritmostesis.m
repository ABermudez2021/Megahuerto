%% Proyecto Integrador

%% **********************************************************************
%% Manipulacion de tabla para graficar y analizar datos adquiridos.
%  **********************************************************************
feeds.created_at.Format='dd.MM.uuuu HH:mm';
feeds.entry_id.Format='dd.MM.uuuu HH:mm';
tiempo1=feeds.created_at+timeofday(feeds.entry_id);
Data1=tabla(feeds.tiempo1,feeds.field3);  
figure();plot(Data1.tiempo1, Data1.Var2);% la columna de campo 3 
                                         %adquiere nombre Var2
%% **********************************************************************
%% Grafica de datos adquiridos.
%  **********************************************************************
Data1=table(tiempo1,lastfeeds.field3,lastfeeds.field4,lastfeeds.field5,lastfeeds.field6);
figure(1);plot(Data1.tiempo1,Data1.Var2)
title('Adquisicion de Datos desde Thingspeak')
ylabel('% Humedad')
hold on
grid on
grid minor
plot(Data1.tiempo1,Data1.Var3)
plot(Data1.tiempo1,Data1.Var4)
plot(Data1.tiempo1,Data1.Var5)
legend('Hum Suelo 1','Hum Suelo 2','Hum Suelo 3','Temp Aire','Location','Southwest')
%% **********************************************************************
%% Manipulacion para utilizar los datos provenientes de algoritomo Matlab
%  **********************************************************************
tiempo=datetime(volt1(:,3),'ConvertFrom','datenum');
tiempoid(15000,1) = '29-Aug-2021 04:29:58';%cambio ultimo bloque xq es cero
%CAMBIAR LA ULTIMA FILA XQ ES CERO Y DAÑA LA GRAFICA Y DATOS
figure(2)
plot(tiempoid,volt1(:,1))
hold on
grid minor
plot(tiempoid,volt2(:,1))
plot(tiempoid,volt3(:,1))
entrada=zeros(15000,1);
entrada(4000:4003,1)=60;
entrada(8000:8003,1)=60;
entrada(12000:12003,1)=60;
plot(tiempoid,entrada)
ylim([40 80])
legend('Hum 1','Hum 2','Hum 3','Entrada','location','best')
title("Adquisicion de Datos Entrada y 3 Muestras (Salida y/o Retroalimentación)")
ylabel("entrada - % humedad")
%% ************************************************************
%% Filtrado y acondicionamiento de la señal para identificacion
%  ************************************************************
time=[1:1:15000];
time=time';
filtro6=tf(2,[1 2])
dataf6=lsim(filtro6,volt2(:,1),time);
plot(tiempoid,dataf6)

%% *************************************************
%% Generacion de realidad aumentada con ruido blanco
%  *************************************************
data1=volt1(140:190,1);
data2=awgn(data1,24);
data3=awgn(data1,25);
data4=awgn(data1,26);
fata1=[data1;data2;data3;data4];
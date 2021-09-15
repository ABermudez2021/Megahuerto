%%ADQUISICION DE DATOS 2 INTEGRADORA\ 
% procedemos a iniciar una curva de frecuencia, tratamos de promediar
% valores tomados cada segundo, y entonces tomarlo en cuenta para la data.
%%a=arduino('com5','mega2560')
a=arduino('com5','Mega2560')
figure(1)

h=animatedline('color','b');
j=animatedline('color','m');
k=animatedline('color','c');
p=animatedline('color','k');
pj=animatedline('color','y');
ax=gca;
ax.YGrid='on'; 

%ax.YLim=[0 100];

stop=false;
startTime=datetime('now');
maximus=192;
f=0;
c=1;
i=1;
tA=datetime([2020 1 1 00 00 00]);
tB=datetime([2021 1 1 00 00 00]);
timeponow=linspace(tA,tB,maximus);
timeponow=timeponow';
hum=zeros(maximus,1);
volt1=zeros(maximus,3);
volt2=zeros(maximus,3);
volt3=zeros(maximus,3);
promvolt2=zeros(maximus,1);
sumv1=0%(5-readVoltage(a,'A0'))*20;
sumv2=0%(5-readVoltage(a,'A1'))*20;
sumv3=0%(5-readVoltage(a,'A2'))*20;
per=900;
while i<maximus
    
    while f<per
        
        v1=(5-readVoltage(a,'A0'))*20;
        v2=(5-readVoltage(a,'A1'))*20;  
        v3=(5-readVoltage(a,'A2'))*20;
        promV=(v1+v2+v3)/3;
        sumv1=sumv1+v1;
        sumv2=sumv2+v2;
        sumv3=sumv3+v3;
        %addpoints(pj, datenum(t),v2)
        promvolt2(c)=v2;
        f=f+1
        c=c+1
        i
        %drawnow
        pause(1)
        if(mod(i,40)==0&mod(f,100)==0)
            writePWMDutyCycle(a,'D5',0.90)
            pause(8)
        else
            writePWMDutyCycle(a,'D5',0);
        end
    end 
    f=0;
    
    pv1=sumv1/(per)
    pv2=sumv2/(per)
    pv3=sumv3/(per)
    sumv1=0;
    sumv2=0;
    sumv3=0;
    t=datetime('now')-startTime;
    
    hum(i)=promV;
    volt1(i,1)=pv1;
    volt2(i,1)=pv2;
    volt3(i,1)=pv3;
    volt1(i,2)=datenum(t);
    volt2(i,2)=datenum(t);
    volt3(i,2)=datenum(t);
    volt1(i,3)=datenum(datetime('now'));
    volt2(i,3)=datenum(datetime('now'));
    volt3(i,3)=datenum(datetime('now'));
    %timepo(i)=t;
    timeponow(i)=datetime('now');
    i=i+1
    addpoints(h,datenum(t),pv1)
    addpoints(j, datenum(t),pv2)
    addpoints(k, datenum(t),pv3)
    %%addpoints(p, datenum(t),promV)

    drawnow
     
    %pause(1)
end
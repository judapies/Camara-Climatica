%% Pruebas Celda de Peltier Incubadora
%% Prueba 1. Disipador que habia en la Oficina
clc,clear
T=[21.6  20.8 20.8 20.7 20.6 20.7 20.7 20.7 20.8 20.7 20.6 20.5 20.4 20.3 20.3 20.2 20.1 19.9 19.7 19.6 19.5 ...
    19.5 19.6 19.5 19.5 19.4 19.1 19.0 18.9 18.9 17.9 17.8 17.8 17.7];
Hora=[930 931 933  934  935  937  939  941  944  945  947  948  950  951  952  954  955  959  1005 1010 1012 ...
    1016 1020 1022 1024 1026 1042 1056 1105 1116 1139 1141 1144 1149];
plot(Hora,T,'k');
x=length(Hora);
xlim([930 Hora(x)])
ylim([0 25])
%legend('Temperatura Cubo Plastico' );
grid on


%% Prueba 2. Disipador que habia en equipo enfriador de agua
clc,clear
T=   [19.3 19.1 18.3 18.2 18.0 17.0 16.9 16.9 16.6 16.4];
Hora=[1224 1225 1230 1231 1233 1304 1306 1312 1315 1330];
plot(Hora,T,'k');
x=length(Hora);
xlim([1224 Hora(x)])
ylim([0 25])
%legend('Temperatura Cubo Plastico' );
grid on

%% Prueba 3. Disipador que habia en equipo enfriador de agua
clc,clear
T=   [20.6 20.7 21.3 ];
Hora=[1436 1437 1441 ];
plot(Hora,T,'k');
x=length(Hora);
xlim([1436 Hora(x)])
ylim([0 25])
%legend('Temperatura Cubo Plastico' );
grid on
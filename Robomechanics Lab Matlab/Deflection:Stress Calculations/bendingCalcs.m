function trhex = bendingCalcs(G, E, color, name)
%Jesse Wallace, Robomechanics Lab, Feb. 2020
%Variables should be material - relevant. 
%This function plots deflection due to applied moment on free end as well
%as applied transverse force.
%This function also plots stress v strain curves for various materials for comparison 
%TODO: Double check units 


%variables - leg geometry and constants 
radius = 0.0515; %Meters, assumed
%Ft = [0 : 10 : 100]'; %Newtons
Ft = linspace(0, 100); 
C = 1.5; %correction factor for rectangluar cross section
Acs = (0.01)*(0.00207); %Meters squared, area of rectangular cross section
dtheta = [0 : pi/5 : 2*pi - pi/5];
h = 1; %thickness, m 
b = 1; %cross-sectional height 
I = (h*b^3)/12; %Area moment of inertia 


%Calculated deflection based on applied moment on free end
defm = (pi*radius^3.*Ft)/(2*E*I);

figure(1)
hold on
plot(Ft, defm)
legend(name)
hold off
xlabel("Applied Load, N") 
ylabel("Deflection, Moment, m") 
title("Deflection plotted with varying applied moment")

%Calculated deflection based on applied transverse force 
deft = (pi*C*radius.*Ft)/(2*Acs*G);

figure(2) 
hold on;
plot(Ft, deft, color)
hold off;
xlabel("Applied Load, N") 
ylabel("Deflection, Transverse Load, m")
title("Deflection plotted with varying applied transverse load")

%calculated net deflection 
defn = (pi*radius^3.*Ft)/(2*E*I);

figure(3) 
hold on; 
plot(Ft, defn, color)
hold off; 
xlabel("Applied Load, N") 
ylabel("Net Deflection, m")
title("Net deflection with respect to net applied load") 

%Calculated Strain, usig diameter(?) 
strain = defn./(2*radius); %calculates average strain 

%Calculated Stress 
stress = E.*strain; 

figure(4)
hold on;
plot(strain, stress, color) 
hold off;
xlabel("Strain") 
ylabel("Stress, Pa")
title("Stress-Strain curve comparison for all materials")

%Strain Energy 
SE = ((Ft.^2).*radius)/(2*Acs*E);

fig5 = figure(5) 
hold on; 
plot(Ft, SE)
hold off;
xlabel("Force, Newtons") 
ylabel("Strain Energy, Joules") 
title("Strain Energy plotted with varying tensile load")

%Jesse Wallace
%1/22/2020
%Matlab Simulation of various loads on a T-Rhex leg configuration 

%Strain Calculations

%variables 
radius = 0.08; %Meters, assumed 
Ft = [0 : 10 : 100]'; %Newtons
C = 1.5; %correction factor for rectangluar cross section
Gacrylic = 1.7; %GPa, modulus of rigidity for acrylic
Acs = 0.08; %Meters squared, area of rectangular cross section
dtheta = [0 : pi/5 : 2*pi - pi/5];
mass = 0.02 %kg 
I = (mass*radius^2)/2 %inertia of a semicircular object
E = 
%tensilestress
%tensilestrain 


%Change in energy due to applied tansverse force, N
deltaU = (C*radius*Ft*dtheta)/(2*Acs*Gacrylic); 

%plot(Ft, deltaU)

%Calculated approximate deflection based on applied moment on free end
delta = (pi*(radius^3.*Ft)/(2*E*I)

%Calculated approximate deflection based on applied transverse force 
delta = (pi*(radius^3.*Ft)/(2*E*I)


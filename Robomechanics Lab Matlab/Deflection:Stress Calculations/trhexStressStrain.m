%Jesse Wallace
%1/22/2020
%Robomechanics Lab - TRhex - Vertical Mobility using Microspines
%Matlab Simulation of various loads on a T-Rhex leg configuration 
%Plots deflection due to applied moment on free end as well as applied transverse force 
%plots stress v strain curves for various materials for comparison 
%TODO: Double check units 


%variables - leg geometry and constants 
radius = 0.0515; %Meters, assumed
Ft = [0 : 10 : 100]'; %Newtons
C = 1.5; %correction factor for rectangluar cross section
Acs = (0.01)*(0.00207); %Meters squared, area of rectangular cross section
dtheta = [0 : pi/5 : 2*pi - pi/5];
mass = 5.12*10^(-3); %kg - SHOUDLNT DEPEND ON MASS 
I = (mass*radius^2)/2 %bending moment, NOT mass dependent ****


%material properties 
%High - Impact Acrylic
Ga = 1.7; %GPa, modulus of rigidity for acrylic(shear modulus)
Ea = 2203; %Elastic Modulus, MPa

%Delrin 
Gd = 3.1 %GPa, shesr modulus of Delrin(450,000 psi)
Ed = 24131.65 %Elastic Modulus of Delrin, MPa

%Calculated deflection based on applied moment on free end
defma = (pi*radius^3.*Ft)/(2*Ea*I);
defmd = (pi*radius^3.*Ft)/(2*Ed*I);

figure(1)
plot(Ft, defma)
hold on 
plot(Ft, defmd) 
hold off
xlabel("Applied Load, N") 
ylabel("Deflection, Moment, m") 

%Calculated deflection based on applied transverse force 
defta = (pi*C*radius.*Ft)/(2*Acs*Ga);
deftd = (pi*C*radius.*Ft)/(2*Acs*Gd);

figure(2) 
plot(Ft, defta, "r")
hold on 
plot(Ft, deftd) 
hold off
xlabel("Applied Load, N") 
ylabel("Deflection, Transverse Load, m")

%calculated net deflection 
defna = (pi*radius^3.*Ft)/(2*Ea*I);
defnd = (pi*radius^3.*Ft)/(2*Ed*I);

figure(3) 
plot(Ft, defna, "g")
hold on
plot(Ft, defnd)
hold off
xlabel("Applied Load, N") 
ylabel("Net Deflection, m")

%Calculated Strain, usig diameter(?) 
straina = defna./(2*radius);
straind = defnd./(2*radius);

%Calculated Stress 
stressa = Ea.*straina;
stressd = Ed.*straind;

figure(4)
plot(straina, stressa, "go") 
hold on
plot(straind, stressd, "bo")
hold off 
xlabel("Strain") 
ylabel("Stress, Pa")




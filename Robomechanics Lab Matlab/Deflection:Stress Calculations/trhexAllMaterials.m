%Jesse Wallace
%2/6/2020
%Robomechanics Lab - TRhex - Vertical Mobility using Microspines
%Matlab Simulation of various loads on a T-Rhex leg configuration 
%Plots deflection due to applied moment on free end as well as applied
%transverse force 
%plots stress v strain curves for various materials for comparison 


%INPUTS ARE SHEAR MODULUS, ELASTIC MODULUS AND COLOR

%High-Impact Acrylic
bendingCalcs(1.7,2203, "y", "High Impact Acrylic")

%Delrin
bendingCalcs(3.1, 24131.65, "m", "Delrin")

%Aluminum Alloy 
bendingCalcs(27, 70000, "c", "Aluminum alloy")

%Spring Steel 
bendingCalcs(78, 207000, "r", "Spring Steel")

%Carbon Fiber Tubing 
bendingCalcs(12.5, 22800, "g", "Carbon Fiber Tubing")

%PLA Plastic 
bendingCalcs(2.4, 3500, "b", "PLA Plastic") 

%Plexiglas
bendingCalcs(1.151, 2413.16, "w", "Plexiglas") 

%Fiberglass





ylim([-1 5]);
ylim([0 2.5]); 

hold off
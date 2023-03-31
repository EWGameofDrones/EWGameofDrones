function coord = initCoord(port)
%INITCOORD Summary of this function goes here
%   Detailed explanation goes here
delete(instrfindall); % gets rid of weird memory error

%% establishes serial port
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% THIS IS A LINE OF CODE THAT WILL NEED TO BE CHANGED %%%%%%%%
%value = char(app.PortDropDown.Value);
%disp(value);
%coord = serialport('/dev/tty.usbmodem141101', 115200);
%coordArduino = serialport(value, 115200);
disp(port);
coord = serialport(port, 115200);
disp(coord);
%disp(coordArduino);
% This serial port declaration will be different on different computers,
% specifically the 'COM14' portion. 

configureTerminator(coord,'LF'); % end each sent packet with a newline char

%% flush any trash in buffer before main operations start
flush(coord); 
end


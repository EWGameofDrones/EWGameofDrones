function coordArduino = initArduino
%% Function Description
% This function is the very first to run upon the start-up of the GUI. This
% function is the Arduino Initialization that establishes a serial
% communication with the Coordinator Arduino. The Coordinator needs to be
% plugged into the computer via USB and its COM port needs to be
% identified. 
%
% This serial communication will serve as MATLAB's 2-way Read and Write 
% to/from the Coordinator Arduino. These read/write comms will be in string
% arrays, which mimmick the Xbee's packet framework, and the string arrays 
% will be referred to as packets elsewhere in the code. The MATLAB-Arduino 
% comms will be on a 115200 Baud rate (different from the 9600 on Arduino 
% alone) and all packets are terminated with a newline character. 
%

%%
%clc; % clear command window

delete(instrfindall); % gets rid of weird memory error

%% establishes serial port
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% THIS IS A LINE OF CODE THAT WILL NEED TO BE CHANGED %%%%%%%%
%value = char(app.PortDropDown.Value);
%disp(value);
coordArduino = serialport('/dev/tty.usbmodem141101', 115200);
%coordArduino = serialport(value, 115200);
%disp(coordArduino);
% This serial port declaration will be different on different computers,
% specifically the 'COM14' portion. 

configureTerminator(coordArduino,'LF'); % end each sent packet with a newline char

%% flush any trash in buffer before main operations start
flush(coordArduino); 

end

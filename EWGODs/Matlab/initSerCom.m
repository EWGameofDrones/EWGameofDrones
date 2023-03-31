function zigB = initSerCom
% clc;
% clear all;

delete(instrfindall);

zigB = serialport('COM5', 9600);

configureTerminator(zigB,'LF'); %end each sent packet with a newline char

flush(zigB);


end

function writeToXbee(app)
%% add description here
%%
% clc;
% clear all;
% 
% delete(instrfindall);
% 
% zigB = serialport('COM5', 9600);
% 
% configureTerminator(zigB,'LF'); %end each sent packet with a newline char
% 
% flush(zigB);

%%
header = "H"; %establishes a initial char to start each packet with

T = posixtime(datetime('now'));

%T = string(T);

currTimePacket = header + "," + T;

%%
%while(1)
    
    %send this packet every 1 seconds if inside a loop
    writeline(app.zigB, currTimePacket); 
    pause(1); 
    flush(app.zigB);
    
%end

end

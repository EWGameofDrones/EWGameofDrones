function syncTime(app)
%% Function Description
% This function writes from the MATLAB GUI to the Coordinator Arduino via
% the Serial COM Port. The data sent to the Arduino is the real-world clock
% time in the form of the number of seconds (including fractional seconds) 
% elapsed since 00:00:00 1-Jan-1970 UTC (Universal Coordinated Time), 
% ignoring leap seconds (POSIX time). 
%
% The data packet consists of a header and the posix time bound together in
% a string array. The header for the packet is "T" so that the Arduino can
% recognize the specific kind of packet sent. The Arduino then uses this 
% to light up an LED. 

%% create time packet to send to Arduino
header = "T"; %establishes a initial char to start each packet with

T = posixtime(datetime('now'));

currTimePacket = header + "," + T;

%% send packet to Arduino Coordinator

writeline(app.zigB, currTimePacket); 
pause(1); 
flush(app.zigB);

end

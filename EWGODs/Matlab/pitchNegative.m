function pitchNegative(app)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
%% create packet to send to Arduino
header = "D"; %establishes a initial char to start each packet with

disp("PITCH--");
currPacket = header; % add on items to the packet as needed using "+"
%% send packet to Arduino Coordinator

writeline(app.zigB, currPacket); 
pause(1); 
flush(app.zigB);
end

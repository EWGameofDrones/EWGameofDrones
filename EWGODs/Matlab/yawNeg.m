function yawNeg(app)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
%% create packet to send to Arduino
header = "L"; %establishes a initial char to start each packet with

disp("YAW--");
currPacket = header; % add on items to the packet as needed using "+"
%% send packet to Arduino Coordinator

writeline(app.zigB, currPacket); 
pause(1); 
flush(app.zigB);
end


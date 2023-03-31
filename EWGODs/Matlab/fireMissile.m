function fireMissile(app)
%% Function Description
% This function writes from the MATLAB GUI to the Coordinator Arduino via
% the Serial COM Port. The data sent to the Arduino is currently nothing
% except the header for the packet. This is an open ended function intended
% to replace the phone app currently being used to operate the turret.
% 
% The data packet consists of a header and (insert code) bound together in
% a string array. The header for the packet is "F" so that the Arduino can
% recognize the specific kind of packet sent. The Arduino then uses this 
% to light up an LED. 


%% create packet to send to Arduino
header = "F"; %establishes a initial char to start each packet with

% put code for what to send in the packet here
%%%%%%%%%%%%%%
%%%%%%%%%%%%%%

currPacket = header; % add on items to the packet as needed using "+"

%% send packet to Arduino Coordinator

writeline(app.zigB, currPacket); 
pause(1); 
flush(app.zigB);

end


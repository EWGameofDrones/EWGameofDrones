function armMissile(app)
%% Function Description
% This function writes from the MATLAB GUI to the Coordinator Arduino via
% the Serial COM Port. The data sent to the Arduino is the state of the 
% toggle switch inside the GUI. The ARM toggle can be set to 'On' or 'Off'
% and upon a change of this state, the data packet will either append a 1
% or a 0 integer to signal an on or off state to the Arduino. 
%
% The data packet consists of a header and the toggle state bound together 
% in a string array. The header for the packet is "A" so that the Arduino 
% can recognize the specific kind of packet sent. The Arduino then uses 
% this to light up an LED. 



%% create toggle state packet to send to Arduino
header = "A"; %establishes a initial char to start each packet with

toggleState = app.ARMSwitch.Value;

tSS = 0;

if strcmp(toggleState, 'On')
    app.FIREButton.BackgroundColor = [1 0 0];
    tSS = 1;
else
    tSS = 0;
    app.FIREButton.BackgroundColor = [0.5 0.5 0.5];
end

currPacket = header + "," + tSS;

%% send packet to Arduino Coordinator

writeline(app.zigB, currPacket); 
pause(1); 
flush(app.zigB);

end

function setArduinoClockTime(app)


header = "H"; %establishes a initial char to start each packet with

timeIndc = "T";

    
%send this packet every 1 seconds if inside a loop
%writeline(app.zigB, header); 



pause(1); 
flush(app.zigB);
    


end


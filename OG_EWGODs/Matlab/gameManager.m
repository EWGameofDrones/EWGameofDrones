function gameManager(app)
%GAMEMANAGER Summary of this function goes here
%   Detailed explanation goes here

%planning

%preStartup

%waiting

%running

%end
%Welcome msg
disp("WELCOME TO EW GAME OF DRONES");

app.PortDropDown.Items = serialportlist("available");

app.gameState = 'startUp';
%disp(app.gameState);
disp("START button defaul VALUE");
disp(app.STARTButton.Value);
port = char(app.PortDropDown.Value);
disp(port);
while app.run
    switch app.gameState
        case 'startUp'
            %setup coordinator
            
            disp('starting');
            if app.STARTButton.Value == 1
                try
                    disp('connecting');
                    port = char(app.PortDropDown.Value);
                    disp(port);
                    app.zigB = initCoord(port);
                    %disp(app.zigB);
                    app.gameState = 'running';
                catch
                end
            
            else
                pause(19);
                disp("connection failed...");
                disp("attempting to reconnect...");
            end
        case 'running'
            disp('running');
            try
                readFromXbee(app);
            catch
            end
        case 'end'
            if app.STARTButton.Value == 0
                app.run = false;
            end
            
    end
    
end

    

end


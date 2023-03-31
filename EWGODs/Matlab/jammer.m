function jammer(app,distance)
%DRONEPOSITION Summary of this function goes here
%   Detailed explanation goes here
tempX = 0;
tempY = 0;
droneX = 35;
droneY = 35;
foundSensor = false;
offsetX = 26.6;
offsetY = 12.6;
multiplier = 1;
tempSensor = 0;
disp("in function");
if distance > 100
    multiplier = distance ./ 30.48;
else
    if distance > 0 && distance <= 30.48
        multiplier = 1.5;
    elseif distance > 30.48 && distance <= 60.96
        multiplier = 2;
    elseif  distance > 60.96 && distance <= 91.44
        multiplier = 2.5;
    else
        multiplier = 3;
    end
end

sensorListLength = length(app.sensorList);

if sensorListLength >= 1
    for i = 1:length(app.sensorList)
        disp(app.sensorList(i).Id);
        tempSensor = app.sensorList(i).Id;
        disp(tempSensor);
        tempX = str2double(app.sensorList(i).XPos);
        tempY = str2double(app.sensorList(i).YPos);
        tempOrientation = app.sensorList(i).Orientation;
            
        if strcmp(tempSensor, '1')
            disp("GETS INTO Sensor 1 check");
            
            if strcmp(tempOrientation, "South")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
                
                set(app.DroneImg,'Position', [droneX droneY 45 43]);
                set(app.DroneImg,'visible','on');
            elseif strcmp(tempOrientation, "North")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
                
                set(app.DroneImg,'Position', [droneX droneY 45 43]);
                set(app.DroneImg,'visible','on');
            elseif strcmp(tempOrientation, "East")
                droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg,'Position', [droneX droneY 45 43]);
                set(app.DroneImg,'visible','on');
            else
                droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg,'Position', [droneX droneY 45 43]);
                set(app.DroneImg,'visible','on');
            end
        elseif strcmp(tempSensor, '2')
            if strcmp(tempOrientation, "South")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
                
                set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_2,'visible','on');
            elseif strcmp(tempOrientation, "North")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
                
                set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_2,'visible','on');
            elseif strcmp(tempOrientation, "East")
                droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_2,'visible','on');
            else
                droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_2,'visible','on');
            end
        elseif strcmp(tempSensor, '3')
            if strcmp(tempOrientation, "South")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
                
                set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_3,'visible','on');
            elseif strcmp(tempOrientation, "North")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
                
                set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_3,'visible','on');
            elseif strcmp(tempOrientation, "East")
                droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_3,'visible','on');
            else
                droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_3,'visible','on');
            end
        elseif strcmp(tempSensor, '4')
            if strcmp(tempOrientation, "South")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
                
                set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_4,'visible','on');
            elseif strcmp(tempOrientation, "North")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
                
                set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_4,'visible','on');
            elseif strcmp(tempOrientation, "East")
                droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_4,'visible','on');
            else
                droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_4,'visible','on');
            end
            
        elseif strcmp(tempSensor, '7')
            
            if strcmp(tempOrientation, "South")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
                
                set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_7,'visible','on');
            elseif strcmp(tempOrientation, "North")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
                
                set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_7,'visible','on');
            elseif strcmp(tempOrientation, "East")
                droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_7,'visible','on');
            else
                droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_7,'visible','on');
            end
        elseif strcmp(tempSensor, '8')
            if strcmp(tempOrientation, "South")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
                set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_8,'visible','on');
            elseif strcmp(tempOrientation, "North")
                droneX = (tempX .* offsetX) + 33;
                droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
                
                set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_8,'visible','on');
            elseif strcmp(tempOrientation, "East")
                droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_8,'visible','on');
            else
                droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
                droneY = (tempY .* offsetY) + 35;
                
                set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
                set(app.DroneImg_8,'visible','on');
            end
        end
        
    end
    pause(0.2);
    %%OFF loop
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','off');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','off');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','off');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','off');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','off');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','off');
        end
    end
    
end
    pause(0.2);
    %%ON loop
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','on');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','on');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','on');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','on');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','on');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','on');
        end
    end
    pause(0.2);
    %%OFF loop
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','off');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','off');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','off');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','off');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','off');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','off');
        end
    end
    pause(0.2);
    %%ON loop
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','on');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','on');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','on');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','on');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','on');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','on');
        end
    end
    pause(0.2);

    %%OFF loop
    pause(0.2);
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','off');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','off');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','off');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','off');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','off');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','off');
        end
    end
    
    %%ON loop
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','on');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','on');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','on');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','on');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','on');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','on');
        end
    end
    pause(0.2);
    pause(1.0);
    %%OFF loop
    for i = 1:length(app.sensorList)
        tempSensor = app.sensorList(i).Id;
        if strcmp(tempSensor, '1')
            set(app.DroneImg,'visible','off');
        elseif strcmp(tempSensor, '2')
            set(app.DroneImg_2,'visible','off');
        elseif strcmp(tempSensor, '3')
            set(app.DroneImg_3,'visible','off');
        elseif strcmp(tempSensor, '4')
            set(app.DroneImg_4,'visible','off');
        elseif strcmp(tempSensor, '7')
            set(app.DroneImg_7,'visible','off');
        elseif strcmp(tempSensor, '8')
            set(app.DroneImg_8,'visible','off');
        end
    end
    
end


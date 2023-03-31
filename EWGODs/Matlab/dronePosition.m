function dronePosition(app, id, distance)
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
        if strcmp(id, app.sensorList(i).Id)
            
            tempX = str2double(app.sensorList(i).XPos);
            tempY = str2double(app.sensorList(i).YPos);
            tempOrientation = app.sensorList(i).Orientation;
            
            foundSensor = true;
            
        end
        
    end
    
end

if foundSensor
    if strcmp(id, '1')
        disp("sensor 1");
        
        if strcmp(tempOrientation, "South")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
            set(app.DroneImg,'visible','on');
            set(app.DroneImg,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "North")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
            set(app.DroneImg,'visible','on');
            set(app.DroneImg,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "East")
            droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg,'visible','on');
            set(app.DroneImg,'Position', [droneX droneY 45 43]);
        else
            droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg,'visible','on');
            set(app.DroneImg,'Position', [droneX droneY 45 43]);
        end
    elseif strcmp(id, '2')
        if strcmp(tempOrientation, "South")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
            set(app.DroneImg_2,'visible','on');
            set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "North")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
            set(app.DroneImg_2,'visible','on');
            set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "East")
            droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_2,'visible','on');
            set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
        else
            droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_2,'visible','on');
            set(app.DroneImg_2,'Position', [droneX droneY 45 43]);
        end
    elseif strcmp(id, '3')
        if strcmp(tempOrientation, "South")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
            set(app.DroneImg_3,'visible','on');
            set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "North")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
            set(app.DroneImg_3,'visible','on');
            set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "East")
            droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_3,'visible','on');
            set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
        else
            droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_3,'visible','on');
            set(app.DroneImg_3,'Position', [droneX droneY 45 43]);
        end
    elseif strcmp(id, '4')
        if strcmp(tempOrientation, "South")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
            set(app.DroneImg_4,'visible','on');
            set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "North")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
            set(app.DroneImg_4,'visible','on');
            set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "East")
            droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_4,'visible','on');
            set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
        else
            droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_4,'visible','on');
            set(app.DroneImg_4,'Position', [droneX droneY 45 43]);
        end
        
    elseif strcmp(id, '7')
        
        if strcmp(tempOrientation, "South")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
            set(app.DroneImg_7,'visible','on');
            set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "North")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
            set(app.DroneImg_7,'visible','on');
            set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "East")
            droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_7,'visible','on');
            set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
        else
            droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_7,'visible','on');
            set(app.DroneImg_7,'Position', [droneX droneY 45 43]);
        end
    elseif strcmp(id, '8')
        if strcmp(tempOrientation, "South")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 - (offsetY .* multiplier);
            set(app.DroneImg_8,'visible','on');
            set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "North")
            droneX = (tempX .* offsetX) + 33;
            droneY = (tempY .* offsetY) + 35 + (offsetY .* multiplier);
            set(app.DroneImg_8,'visible','on');
            set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
        elseif strcmp(tempOrientation, "East")
            droneX = (tempX .* offsetX) + 33 + (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_8,'visible','on');
            set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
        else
            droneX = (tempX .* offsetX) + 33 - (offsetX .* multiplier);
            droneY = (tempY .* offsetY) + 35;
            set(app.DroneImg_8,'visible','on');
            set(app.DroneImg_8,'Position', [droneX droneY 45 43]);
        end
    end
end



end


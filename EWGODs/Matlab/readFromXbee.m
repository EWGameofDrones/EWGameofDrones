function readFromXbee(app)
%% Function Description:
% This MATLAB function is expecting a string package from the Coordinator Arduino 
% via the Serial COM Port where the values in the packet are as follows:
%
%  |Sensor ID| , |Distance to Nearest Object|, |Current Time|
%     {1,1}    ,          {2,1}              ,       {3,1}        
%
% Each hardware package (sandwich) in the Xbee network all have a
% distinct iD and the sandwichIDs array should reflect that. Each Router
% sandwich sends data packets over to the Coordinator sandwich and the
% Coordinator passes along the data to MATLAB. This function will then take
% that incoming data via the Serial Com Port, validate it, and use 
% the data to populate the GUI components respectively. 
%
% This file also uses the initArduino.m file as a sort of header. The
% entirety of how the code works together can be found in the "Code View" 
% of the GUI display in the EWGODs_Main_Control_Panel.m App Designer file.

%% Const Variables
% Array of sensor/sandwich iDs, modify as you add more %
%tempIDs = [ '1'; '2'; '3'; '4'; '5'; '6' ; '7' ; '8' ; '9']; 
% Changed to numbers for scalability 
sandwichIDs = [ 0; 1; 2; 3; 4; 5; 6; 7; 8; 9; 10];
% Counter to keep track of drones detected
%threat_counter_offset = 0;
%threat_counter = app.DetectionCounterEditField.Value;
%app.DetectionCounterEditField.Value = threat_counter;
%% UIAxes set up old
%Ultrasonic right side postions
%X = [23 23 23];
%Y = [8 12 16];
%Ultrasonic left side positions
%X2 = [0 0];
%Y2 = [8 12];
%RFID Tag reader middle of battle field facing to right side
%X3 = [11.5 11.5];
%Y3 = [5 18];


%ax = app.UIAxes();
%Killzone
%plot(ax, 11.5, 11.5, 'bo', 'MarkerSize', 75,'Color','r','LineWidth',1,'LineStyle','--');
%hold(ax, 'on');
%Nerf Gun positon
%plot(ax, 11.5,11.5, 'Marker','x','MarkerSize', 15, 'Color','r', 'LineWidth',1,'LineStyle','none');
%hold(ax, 'on');
%Ultrasonics right side
%plot(ax, X, Y, 'Marker', '>', 'MarkerSize', 15, 'Color', 'r', 'LineWidth',1,'LineStyle','none');
%hold(ax, 'on');
%Ultrasonics left side
%plot(ax, X2, Y2, 'Marker', '<', 'MarkerSize', 15, 'Color', 'r', 'LineWidth',1,'LineStyle','none');
%hold(ax, 'on');
%RFID middle
%plot(ax, X3,Y3,'Marker','<','MarkerSize', 15,'Color','b','LineWidth',1,'LineStyle','none');
%hold(ax, 'on');
%Lidar left side facing right
%plot(ax, 0,16,'Marker','<','MarkerSize', 15,'Color',[0.4660 0.6740 0.1880],'LineWidth',1,'LineStyle','none');
%hold(ax, 'on');

%plot(ax, 11.5,18,'Marker','x','MarkerSize', 15,'Color','b','LineWidth',1);
%hold(ax, 'on');
%plot(ax, 11.5,5,'Marker','x','MarkerSize', 15,'Color','b','LineWidth',1);
%hold(ax, 'on');
%legend(ax, 'killzone', 'nerf gun', 'ultrasonic', 'ultrasonic', 'rfid','lidar');

%% Drone invisible to start
set(app.DroneImg,'visible','off');
set(app.DroneImg_2,'visible','off');
set(app.DroneImg_3,'visible','off');
set(app.DroneImg_4,'visible','off');
set(app.DroneImg_5,'visible','off');
set(app.DroneImg_6,'visible','off');
set(app.DroneImg_7,'visible','off');
set(app.DroneImg_8,'visible','off');
%%set(app.DroneImg,'Position', [400 110 45 43])

%% No longer in use
% Same value as the Arduino Threshold, used to modify gauge displays. 
% This value MUST be the same as in Arduino AND the GUI Tick Labels & Limits.
%gaugeOffset = 80;

%% Drone default postions old

% Ultrasonic detects drone postions to the left of marker
%set(app.DroneImg,'Position', [585 275 45 43]) %1
%set(app.DroneImg,'Position', [585 215 45 43]) %2
%set(app.DroneImg,'Position', [585 155 45 43]) %3

% Ultrasonic detects drone postions to the right of marker
%set(app.DroneImg,'Position', [60 275 45 43]) %4
%set(app.DroneImg,'Position', [60 215 45 43]) %5
%set(app.DroneImg,'Position', [60 155 45 43]) %6

% RFID detects drone positions to the right of marker
%set(app.DroneImg,'Position', [400 310 45 43]) %7
%set(app.DroneImg,'Position', [400 110 45 43]) %8

%% Main Reading Loop
while(1)
        %% TESTING DROP DOWN
%     if strcmp(app.GameplanDropDown.Value, 'Scenario 3')
%         disp(app.GameplanDropDown.Value);
%     end
    
    
    %disp("Gets into while loop");
    %% get sensor data and save into new string array with no commas
    sensorData = deblank(readline(app.zigB));
    disp(sensorData);
    %% lets convert sensorData from a string array to a char array
    chr = convertStringsToChars(sensorData);
    %% need to check the length of the data we are receiving
    L = strlength(chr);
    %% correct length of data is false automatically
    correctLength = false;
    %% Let's validate this data
    %% Length 3 mean's it got a null packet
    %% Length 13 means it received a ultrasonic sensor packet
    %% Length 14 means it received a RFID RSSI tag packet
    %% If it is not one of those lengths let's double check what is going on
    %% We want a format of x,xx, at the very least
    if(L == 3 || L == 13 || L == 14)
        correctLength = true;
    else
       for i = 1:length(chr)
        if L > 4
            if chr(1) == ',' && chr(4) == ',' %% || chr(5) == ',' the second comma check might not be needed and or can vary 
                disp(chr(i));
                correctLength = true;
            end
        end
       end
    end
    
    %% for testing display the length of the char array
    %disp(L);
    
    
    %% for testing displays the sensor data good or bad
    disp(sensorData);
    
    %% Check if first char in sensorData is a digit
    %% if the first char is not a digit then we received garbage data
    c = strncmp(sensorData, 'digit', 0);
    
    %% for testing
    %%disp(c);
 
    %% If first char is a digit then we process it
    if c && correctLength
        
        sepSenData = split(sensorData, ','); 
        
        %% determine packet validity & fill GUI values
        sensoriD = sepSenData{1,1};
        %turn sensoriD into a temp double for comparison pusposes
        tempID = str2double(sensoriD);
        %disp(tempID);
        
        if ismember(tempID, sandwichIDs)
            % if the packet sent is a null packet %
            %disp(sepSenData{2,1});
            if strcmp(sepSenData{2,1}, 'N') 
                % GUI Values %
                distanceVal = "Not Available";
                timestamp = "Not Available";
                %ledColor = 'g'; % can use 'g' for defining the color green
                %gaugeNum = 0; % this will display as the dial on 80 at resting position, see formula below
                %set(app.DroneImg,'visible','off');
            % if the packet has valid values %
            elseif strcmp(sepSenData{2,1}, 'J')
                distanceVal = "J";
            else 
                distanceVal = sepSenData{2,1}; 
                timestamp = sepSenData{3,1};
                %ledColor = 'r'; % can use 'r' for defining the color red
                %gaugeNum = gaugeOffset - str2num(distanceVal); 
                % Note: This formula converts the default gauge display (low-to-high values)
                % to a more user-intuitive gauge display (high-to-low values)and works 
                % with the any gauge Tick Labels & Tick Limits. 
            end

            %% Populate each GUI field/component with GUI values for specific sensor
            if tempID == 1 
                %% set ultrasonic fields
                app.UltrasonicSensor_Id_EditField_1.Value = sensoriD;
                app.UltrasonicSensor_Distance_EditField_1.Value = distanceVal;
                app.UltrasonicSensor_Time_EditField_1.Value = timestamp;
                temp_distance = str2double(distanceVal);
                %% No longer in use
                %app.Sensor1Lamp.Color = ledColor;
                %app.Sensor1Gauge.Value = gaugeNum;
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg,'visible','off');
                else
%                     threat_counter_offset = threat_counter_offset + 1;
%                     add_check = mod(threat_counter_offset, 5);
%                     if add_check == 0
%                         app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
%                     end
                    dronePosition(app, sensoriD, temp_distance);
                end
            elseif tempID == 2
                %% set ultrasonic fields
                app.UltrasonicSensor_Id_EditField_2.Value = sensoriD;
                app.UltrasonicSensor_Distance_EditField_2.Value = distanceVal;
                app.UltrasonicSensor_Time_EditField_2.Value = timestamp;
                temp_distance = str2double(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_2,'visible','off');
                    %set(app.DroneImg,'Position', [358 421 45 43])
                else
%                     threat_counter_offset = threat_counter_offset + 1;
%                     add_check = mod(threat_counter_offset, 5);
%                     if add_check == 0
%                         app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
%                     end
                    dronePosition(app, sensoriD, temp_distance);
                end
                %% No longer in use
                %app.Sensor2Lamp.Color = ledColor;
                %app.Sensor2Gauge.Value = gaugeNum;
            elseif tempID == 3
                %% set ultrasonic fields
                app.UltrasonicSensor_Id_EditField_3.Value = sensoriD;
                app.UltrasonicSensor_Distance_EditField_3.Value = distanceVal;
                app.UltrasonicSensor_Time_EditField_3.Value = timestamp;
                temp_distance = str2double(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_3,'visible','off');
                else
%                     threat_counter_offset = threat_counter_offset + 1;
%                     add_check = mod(threat_counter_offset, 5);
%                     if add_check == 0
%                         app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
% 
%                     end

                    dronePosition(app, sensoriD, temp_distance);

                end
            elseif tempID == 4
               %% set lidar test fields
                app.UltrasonicSensor_Id_EditField_4.Value = sensoriD;
                app.UltrasonicSensor_Distance_EditField_4.Value = distanceVal;
                app.UltrasonicSensor_Time_EditField_4.Value = timestamp;
                
                %app.SSRT_Time_EditField_2.Value = timestamp;
                temp_distance = str2double(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_4,'visible','off');
                elseif temp_distance > 600
                    set(app.DroneImg_4,'visible','off');
                    distanceVal = 'Not Available';
                    
                    app.UltrasonicSensor_Distance_EditField_4.Value = distanceVal;
                elseif temp_distance > 400
%                     threat_counter_offset = threat_counter_offset + 1;
%                     add_check = mod(threat_counter_offset, 5);
%                     if add_check == 0
%                         app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
%                     end
                    dronePosition(app, sensoriD, temp_distance);
                else
%                     threat_counter_offset = threat_counter_offset + 1;
%                     add_check = mod(threat_counter_offset, 5);
%                     if add_check == 0
%                         app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
%                     end
                    dronePosition(app, sensoriD, temp_distance);
                end
            elseif tempID == 5
                %% set ultrasonic fields
                app.UltrasonicSensor_Id_EditField_5.Value = sensoriD;
                app.UltrasonicSensor_Distance_EditField_5.Value = distanceVal;
                app.UltrasonicSensor_Time_EditField_5.Value = timestamp;
                temp_distance = str2double(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_5,'visible','off');
                else
%                     threat_counter_offset = threat_counter_offset + 1;
%                     add_check = mod(threat_counter_offset, 5);
%                     if add_check == 0
%                         app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
%                     end
                    dronePosition(app, sensoriD, temp_distance);
                end
            elseif tempID == 6
                %% set ultrasonic fields
                app.UltrasonicSensor_Id_EditField_6.Value = sensoriD;
                app.UltrasonicSensor_Distance_EditField_6.Value = distanceVal;
                app.UltrasonicSensor_Time_EditField_6.Value = timestamp;
                temp_distance = str2double(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_6,'visible','off');
                else
%                     app.ThreatsDetectedEditField.Value = app.ThreatsDetectedEditField.Value + 1;
                    dronePosition(app, sensoriD, temp_distance);
                end
            elseif tempID == 7
                %% set rfid fields
                app.SSRT_Id_EditField_1.Value = sensoriD;
                app.SSRT_Rssi_EditField_1.Value = distanceVal;
                app.SSRT_Time_EditField_1.Value = timestamp;
                temp_distance = str2double(distanceVal);
                disp(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_7,'visible','off');
                    %set(app.DroneImg,'Position', [358 421 45 43])
                else
                    dronePosition(app, sensoriD, temp_distance);
                end
            elseif tempID == 8
                %% set rfid fields
                app.SSRT_Id_EditField_2.Value = sensoriD;
                app.SSRT_Rssi_EditField_2.Value = distanceVal;
                app.SSRT_Time_EditField_2.Value = timestamp;  
                temp_distance = str2double(distanceVal);
                %% set drone visibility/position
                if strcmp(distanceVal, 'Not Available') 
                    set(app.DroneImg_8,'visible','off');
                    %set(app.DroneImg,'Position', [358 421 45 43])
                else

                    dronePosition(app, sensoriD, temp_distance);
                end
            elseif tempID == 9
                %% set lidar test fields
                %app.Lidar_Id_EditField.Value = sensoriD;
                %app.Lidar_Distance_EditField.Value = distanceVal;
                %app.SSRT_Time_EditField_2.Value = timestamp; 
            elseif tempID == 0
                %%Test for "jamming"
                if strcmp(distanceVal, 'J')
                    disp("JAMMING PACKET RECEIVED");
                    
                    jammer(app,99);
%FIX JAMMER func
%                     set(app.DroneImg,'Position', [585 275 45 43]); %1
%                     set(app.DroneImg_2,'Position', [585 215 45 43]); %2
%                     set(app.DroneImg_3,'Position', [585 155 45 43]); %3
%                     set(app.DroneImg_4,'Position', [60 275 45 43]); %4
%                     set(app.DroneImg_5,'Position', [60 215 45 43]); %5
%                     set(app.DroneImg_6,'Position', [60 155 45 43]); %6
%                     set(app.DroneImg_7,'Position', [400 310 45 43]); %7
%                     set(app.DroneImg_8,'Position', [400 110 45 43]); %8
%                    
%                     
%                     set(app.DroneImg,'visible','on');
%                     set(app.DroneImg_2,'visible','on');
%                     set(app.DroneImg_3,'visible','on');
%                     set(app.DroneImg_4,'visible','on');
%                     set(app.DroneImg_5,'visible','on');
%                     set(app.DroneImg_6,'visible','on');
%                     set(app.DroneImg_7,'visible','on');
%                     set(app.DroneImg_8,'visible','on');
%                     pause(0.2);
%                     set(app.DroneImg,'visible','off');
%                     set(app.DroneImg_2,'visible','off');
%                     set(app.DroneImg_3,'visible','off');
%                     set(app.DroneImg_4,'visible','off');
%                     set(app.DroneImg_5,'visible','off');
%                     set(app.DroneImg_6,'visible','off');
%                     set(app.DroneImg_7,'visible','off');
%                     set(app.DroneImg_8,'visible','off');
%                     set(app.DroneImg,'visible','on');
%                     set(app.DroneImg_2,'visible','on');
%                     set(app.DroneImg_3,'visible','on');
%                     set(app.DroneImg_4,'visible','on');
%                     set(app.DroneImg_5,'visible','on');
%                     set(app.DroneImg_6,'visible','on');
%                     set(app.DroneImg_7,'visible','on');
%                     set(app.DroneImg_8,'visible','on');
%                     pause(0.2);
%                     set(app.DroneImg,'visible','off');
%                     set(app.DroneImg_2,'visible','off');
%                     set(app.DroneImg_3,'visible','off');
%                     set(app.DroneImg_4,'visible','off');
%                     set(app.DroneImg_5,'visible','off');
%                     set(app.DroneImg_6,'visible','off');
%                     set(app.DroneImg_7,'visible','off');
%                     set(app.DroneImg_8,'visible','off');
%                     set(app.DroneImg,'visible','on');
%                     set(app.DroneImg_2,'visible','on');
%                     set(app.DroneImg_3,'visible','on');
%                     set(app.DroneImg_4,'visible','on');
%                     set(app.DroneImg_5,'visible','on');
%                     set(app.DroneImg_6,'visible','on');
%                     set(app.DroneImg_7,'visible','on');
%                     set(app.DroneImg_8,'visible','on');
%                     pause(0.2);
%                     set(app.DroneImg,'visible','off');
%                     set(app.DroneImg_2,'visible','off');
%                     set(app.DroneImg_3,'visible','off');
%                     set(app.DroneImg_4,'visible','off');
%                     set(app.DroneImg_5,'visible','off');
%                     set(app.DroneImg_6,'visible','off');
%                     set(app.DroneImg_7,'visible','off');
%                     set(app.DroneImg_8,'visible','off');
%                     pause(0.2);
%                     set(app.DroneImg,'visible','on');
%                     set(app.DroneImg_2,'visible','on');
%                     set(app.DroneImg_3,'visible','on');
%                     set(app.DroneImg_4,'visible','on');
%                     set(app.DroneImg_5,'visible','on');
%                     set(app.DroneImg_6,'visible','on');
%                     set(app.DroneImg_7,'visible','on');
%                     set(app.DroneImg_8,'visible','on');
%                     pause(0.2);
%                     pause(1.0);
%                     set(app.DroneImg,'visible','off');
%                     set(app.DroneImg_2,'visible','off');
%                     set(app.DroneImg_3,'visible','off');
%                     set(app.DroneImg_4,'visible','off');
%                     set(app.DroneImg_5,'visible','off');
%                     set(app.DroneImg_6,'visible','off');
%                     set(app.DroneImg_7,'visible','off');
%                     set(app.DroneImg_8,'visible','off');

                end
                
            end
            % To add on more sensor/Router packages here, simply add another 
            % elsif, copy and paste the entire "app. ..." paragraph, and then 
            % replace all numbers with the corresponding sensor iD. Make sure 
            % sensor iDs match in both sandwichIDs array (see above) AND the 
            % GUI components AND the Arduino code sent over for each sandwich.


        end
    end
    %% flush buffer & update GUI clock time
    
    flush(app.zigB);
    
    app.SystemTimeEditField.Value = string(datetime('now'));
    

end 

end
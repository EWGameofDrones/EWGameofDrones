classdef sensor
    %OBJECT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        Id 
        Type
        Orientation
        XPos
        YPos
        
    end
    
    methods
        function obj = sensor(id,type,orientation,x,y)
            %OBJECT Construct an instance of this class
            %   Detailed explanation goes here
            obj.Id = id;
            obj.Type = type;
            obj.Orientation = orientation;
            obj.XPos = x;
            obj.YPos = y;
        end
        
%         function disp(self)
%             %METHOD1 Summary of this method goes here
%             %   Detailed explanation goes here
%             fprintf(...
%                 ['Sensor -',...
%                 ' Id: ' '%s', ...
%                 ' Type: ' '%s',...
%                 ' Orientation: ' '%s',...
%                 ' Position (' '%s' ',' '%s' ')'],...
%                 self.Id,...
%                 self.Type,...
%                 self.Orientation,...
%                 self.XPos, ...
%                 self.YPos);
%         end
    end
end


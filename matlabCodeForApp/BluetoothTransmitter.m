classdef BluetoothTransmitter < handle

    properties
       b % bluetooth object
       start_marker % is attached to the start of the command being sent. Used for verification.
       end_marker % is attached to the end of the command being sent. Used for verification.
       isConnected
    end
    
    methods
        %---constructor-------------------------
        function self = BluetoothTransmitter(varargin)
            % Initial state conditions
            self.isConnected = false;
            self.start_marker = '<';
            self.end_marker = '>';
            
            if (nargin > 0)
                self.b = Bluetooth(varargin{1},1);
            end
            
            if (nargin >= 2)
                if (varargin{2} == 1)
                    fopen(self.b);
                    self.isConnected = true; 
                end
            end
            
        end
        
        %----------------------------
        
        function remoteNames = showDevicesAvailable(self)
            deviceInfo = instrhwinfo('Bluetooth');
            remoteNames = deviceInfo.RemoteNames;
        end
        
        %----------------------------
        
        function connectToDevice(self, nameOfDevice)
            self.b = Bluetooth(nameOfDevice,1);
            fopen(self.b);
            self.isConnected = true;
        end
        
        %----------------------------
        
        function disconnectFromDevice(self)
            fclose(self.b);
            self.isConnected = false;
        end
        
        %----------------------------
        
        function send_commands(self, varargin)
            if (nargin <= 1)
                return;
            end
            
            command = strings;
            command = strcat(command,self.start_marker);
            for i = 2:nargin  
                command = strcat(command,"",num2str(varargin{i-1}));
            end
            command = strcat(command,"",self.end_marker);
            
            fwrite(self.b,command);  
        end
        
         %----------------------------
        
         function echo = receive_binaryData(self)
            echo = fread(self.b,32);
            echo = char(echo');
        end
        
    end
    
end
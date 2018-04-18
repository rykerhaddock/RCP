classdef Robot_CommandCompiler < handle
    
    properties
        rot_speed0
        rot_speed1
        rot_speed2
        rot_speed3
        
        trans_speed0N
        trans_speed1N
        trans_speed2N
        trans_speed3N
        
        trans_speed0E
        trans_speed1E
        trans_speed2E
        trans_speed3E
        
        error_permitted
        distMin_speed1
        distMin_speed2
        distMin_speed3
        
        errorN_last
        errorE_last
        
        state
        previous_state
        k
        i
        j
        
        checking_proposed_destination
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    methods
        
        function self = Robot_CommandCompiler()
            
            self.rot_speed0 = 360/12.5; % deg/s (360 deg in 12.5 s) will implement with larger delays than in speed 1
            self.rot_speed1 = 360/12.5; % deg/s (360 deg in 12.5 s)
            self.rot_speed2 = 360/7.5; % deg/s (360 deg in 7.5 s)
            self.rot_speed3 = 360/5.5; % deg/s (360 deg in 5.5 s)
            self.trans_speed0N = 10/19.3; % ft/s (10 feet in 19.3 s)
            self.trans_speed1N = 10/19.3; % ft/s (10 feet in 19.3 s)
            self.trans_speed2N = 10/12.1; % ft/s (10 feet in 12.1 s)
            self.trans_speed3N = 10/9.0; % ft/s (10 feet in 9.0 s)
            self.trans_speed0E = 10/19.3; % ft/s (10 feet in 19.3 s)
            self.trans_speed1E = 10/31.0; % ft/s (10 feet in 31.0 s)
            self.trans_speed2E = 10/15.0; % ft/s (10 feet in 15.0 s)
            self.trans_speed3E = 10/10.0; % ft/s (10 feet in 10.0 s)
            
            % % % %             % Values used for testing
            % % % %             self.error_permitted = 1;
            % % % %             self.distMin_speed2 = 2;
            % % % %             self.distMin_speed3 = 3;
            
            % Values used for actual application
            self.error_permitted = [1,.25,.05,.01];
            self.distMin_speed1 = .1;
            self.distMin_speed2 = .5;
            self.distMin_speed3 = 1;
            
            
            self.state = 0;
            self.previous_state = 0;
            
            self.i = 0;
            self.j = 0;
            self.k = 1;
            
            self.errorN_last = 0.0;
            self.errorE_last = 0.0;
            
% % %             self.checking_proposed_destination = 0;
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        function cmd = retrieveCommand(self, errorN, errorE)
            
            % Save first position to last position for first request
            if self.i == 0
                self.errorN_last = errorN;
                self.errorE_last = errorE;
                self.i = 1;
            end
            
            
% % % % % % FOR LATER IMPLEMENTATION % % % % % %
% % % % % % % % % % % % % % % % % % % % % % % % %

% % % % %             if (self.checking_proposed_destination == 1)
% % % % %                 self.checking_proposed_destination = 0;
% % % % %                 if (abs(errorN) <= self.error_permitted(4) && abs(errorE) <= self.error_permitted(4))
% % % % %                     self.state = 3;
% % % % %                     self.k = 1;
% % % % %                 else
% % % % %                     self.state = 0;
% % % % %                 end
% % % % %                 
% % % % %             elseif (abs(errorN) <= self.error_permitted(4) && abs(errorE) <= self.error_permitted(4))
% % % % %                 self.checking_proposed_destination = 1;
% % % % % %                 self.state = 2; 
% % % % %                 self.state = 3;
% % % % %             end
            

            if (abs(errorN) <= self.error_permitted(4) && abs(errorE) <= self.error_permitted(4))
                self.state = 3;
                self.k = 1;
            % Check for current error to set necessary self.state
            elseif abs(errorN) <= self.error_permitted(self.k) && self.state==0
                self.state = 1; % fix easting error
            elseif abs(errorE) <= self.error_permitted(self.k) && self.state==1
                if self.k < 4
                    self.k = self.k+1;
                end
                self.state = 0;
            end            
            
            % stop robot between transitions from one state to another
%             if ((self.state ~= self.previous_state) && (self.previous_state ~= 2))
%                 self.state = 2; % get robot to stop
%             end
            
            % update self.previous_state for next function call
            self.previous_state = self.state;
            
            
            
            
            
            % Move to reduce error (northing first, easting second, mark last)
            % self.state = 0 for going north, 1 for going east, 2 for stopped at
            % position to mark
            switch self.state
                case 0
                    if sign(errorN) > 0
                        if abs(errorN) > self.distMin_speed3
                            time_trans = abs(errorN)/self.trans_speed3N;
                            cmd = '3n';
                        elseif abs(errorN) > self.distMin_speed2
                            time_trans = abs(errorN)/self.trans_speed2N;
                            cmd = '2n';
                        elseif abs(errorN) > self.distMin_speed1
                            time_trans = abs(errorN)/self.trans_speed1N;
                            cmd = '1n';
                        else
                            time_trans = abs(errorN)/self.trans_speed0N;
                            cmd = '0n';
                        end
                    else
                        if abs(errorN) > self.distMin_speed3
                            time_trans = abs(errorN)/self.trans_speed3N;
                            cmd = '3s';
                        elseif abs(errorN) > self.distMin_speed2
                            time_trans = abs(errorN)/self.trans_speed2N;
                            cmd = '2s';
                        elseif abs(errorN) > self.distMin_speed1
                            time_trans = abs(errorN)/self.trans_speed1N;
                            cmd = '1s';
                        else
                            time_trans = abs(errorN)/self.trans_speed0N;
                            cmd = '0s';
                        end
                    end
                case 1
                    if sign(errorE) > 0
                        if abs(errorE) > self.distMin_speed3
                            time_trans = abs(errorE)/self.trans_speed3E;
                            cmd = '3e';
                        elseif abs(errorE) > self.distMin_speed2
                            time_trans = abs(errorE)/self.trans_speed2E;
                            cmd = '2e';
                        elseif abs(errorE) > self.distMin_speed1
                            time_trans = abs(errorE)/self.trans_speed1E;
                            cmd = '1e';
                        else
                            time_trans = abs(errorE)/self.trans_speed0E;
                            cmd = '0e';
                        end
                    else
                        if abs(errorE) > self.distMin_speed3
                            time_trans = abs(errorE)/self.trans_speed3E;
                            cmd = '3w';
                        elseif abs(errorE) > self.distMin_speed2
                            time_trans = abs(errorE)/self.trans_speed2E;
                            cmd = '2w';
                        elseif abs(errorE) > self.distMin_speed1
                            time_trans = abs(errorE)/self.trans_speed1E;
                            cmd = '1w';
                        else
                            time_trans = abs(errorE)/self.trans_speed1E;
                            cmd = '0w';
                        end
                    end
                case 2
                    cmd = 's';
                    self.state = 0;
                case 3
                    cmd = 'x';
                    self.state = 0;
            end
            
            
            
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        function updateSpeedLocations(self, speed1LowerBound, speed2LowerBound, speed3LowerBound)
           self.distMin_speed1 = speed1LowerBound;
           self.distMin_speed2 = speed2LowerBound;
           self.distMin_speed3 = speed3LowerBound;
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        function updateErrorPermitted(self, errorAllowance)
           self.error_permitted = errorAllowance;
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        function nextErrorThreshold = getNextOffsetToDestination(self)
            nextErrorThreshold = self.error_permitted(self.k);
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%
        
    end
    
end
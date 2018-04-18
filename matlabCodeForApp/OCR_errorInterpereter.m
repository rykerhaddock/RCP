classdef OCR_errorInterpereter < handle
    
    properties
        rect
        croppedImage
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    methods
        function self = OCR_errorInterpereter()
            
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        function updateRect(self)
            % Now, take a Screen shot, and create a MATLAB image from it
            imageOfScreen = Screen_Capture(0);
            figure(1);
            % crop image and store the size parameters into variable self.rect.
            [self.croppedImage, rect] = imcrop(imageOfScreen);
            self.rect = java.awt.Rectangle(rect(1),rect(2),rect(3),rect(4))
            close;
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        function [faultyErrorReadings, errorN, errorE] = getErrorData(self)
            
            % Assign default values to outputs
            faultyErrorReadings = 0;
            errorN = 0;
            errorE = 0;
            
            self.croppedImage = Screen_Capture(self.rect);
            ocrtxt = ocr(self.croppedImage, 'CharacterSet','0123456789.-N/A');
%             imshow(self.croppedImage)
            
            
            % % %         disp(numel(ocrtxt.Words))
            % % %         word1 = char(ocrtxt.Words(1));
            % % %         word2 = char(ocrtxt.Words(2));
            % % %         word3 = char(ocrtxt.Words(3));
            % % %         word4 = char(ocrtxt.Words(4));
            % % %         disp([word1,' ', word2,' ', word3,' ', word4])
            
            
            % Add verification to make sure that the values obtained by the ocr
            % are correct
            if (numel(ocrtxt.Words) == 4)
                
                % Pull out error data from ocr readings
                errorN_str = char(ocrtxt.Words(2));
                errorE_str = char(ocrtxt.Words(4));
                
                
                % Convert that data to numeric data that can be used.
                errorN_numeric = sscanf(errorN_str,'%lf');
                if (isempty(errorN_numeric))
                    faultyErrorReadings = 1;
                else
                    errorN = errorN_numeric(1);
                end
                
                errorE_numeric = sscanf(errorE_str,'%lf');
                if (isempty(errorE_numeric))
                    faultyErrorReadings = 1;
                else
                    errorE = errorE_numeric(1);
                end
            else
                faultyErrorReadings = 1;
            end
            
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
    end
    
end
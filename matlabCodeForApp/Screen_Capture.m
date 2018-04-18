function newImage = Screen_Capture(rectangle)
robo = java.awt.Robot;
if rectangle == 0
    t = java.awt.Toolkit.getDefaultToolkit();
    rectangle = java.awt.Rectangle(t.getScreenSize());
end
image = robo.createScreenCapture(rectangle);

%Reformat image from bufferedimage to matlab image
h=image.getHeight;
w=image.getWidth;
pixelsData = reshape(typecast(image.getData.getDataStorage, 'uint8'), 4, w, h);
newImage = cat(3, ...
    transpose(reshape(pixelsData(3, :, :), w, h)), ...
    transpose(reshape(pixelsData(2, :, :), w, h)), ...
    transpose(reshape(pixelsData(1, :, :), w, h)));
end
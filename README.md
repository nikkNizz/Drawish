# Drawish  
Is a program for basic drawing.  
It also allows you to modify existing images.

### COMPILE FROM SOURCE (LINUX):
Download and unzip the project.  
Give the following commands from the terminal:

sudo apt update  
sudo apt install g++  
sudo apt install cmake  
sudo apt install qt6-base-dev  
cd /path/to/Drawish_project  
cmake .  
make  
sudo make install  

----------------------------------------
or install and compile directly from Qt Creator on Windows, Mac, or Linux.

### PRECOMPILED PACKAGES:
The compiled program for Linux and Windows can be downloaded from the [releases](https://github.com/nikkNizz/Drawish/releases/tag/v0.4) page of this project.  
For **Windows**, download Drawish_version_Win.zip.  
Extract the folder, and run the .exe file. Create a link to the executable for faster use.  

For **Debian/Linux** download and run the portable Appimage.

#### USE
You can open images from the" File->Open" menu. You can also
paste an image from the clipboard ("Edit->Paste from clipboard") or from a file ("Edit->Paste from file")

---
The drawing options are visible on the left of the drawing area.
The color palette is also visible here.
The first color on the top left is the active color (default black). Change the active color:  
-by clicking on a quick color or on the + button  
-from the "Image->Colors" menu and entering the rgb values ​​manually  
-using the pick color on the image.  

When you select an option it will be active for the drawing area,
when you deselect it, it will no longer be active and any
pending operations will be deleted.

#### Selection
Click the [selection] button to select a
portion of the image. The selection will be detached from the image and
can be dragged with the mouse, and can also be resized by pulling the edges with the mouse.  
To copy the portion **without detaching it**, select it and,
before moving it, go to the "Edit->copy selection (no clipboard) " menu.  
You can also create a selection by entering the coordinates from the keyboard
from the "Image->Sizes->[Create selection]" menu.  
To move the selection finely with the keyboard use the A (left), Q or W (up), S (right), Z or X (down) keys.  
To make the white area of ​​the selection transparent, check the box
from the "Edit->Transparent selection" Menu, **before** creating the selection.  
To select everything, use the "Edit->Select all" menu.  
To save the image to a file select: "Edit->Selection to file".  
To finish and pasting the selection, click on a point on the image.  
To skip pasting, deselect the selection button.

#### Freehand drawing
Click the [marker] button
and move the mouse over the drawing area, holding
down the left mouse button. You can draw
with the keyboard by selecting in the top menu how many pixels to draw
when you press the keys.  
Use the A (left), Q or W (up), S (right), Z or X (down) keys.  
In the panel that appears above the drawing area you can
choose attributes. Do not use transparent color to draw.  
Use white color to erase.

#### Fill
Click the [fill] button and click on the
point in the image where you want to replace a color with the
active color.  The replacement occurs on the exact color that you clicked.  
To perform the replacement by color affinity, move the slider that
appears at the top.  
Increasing the value decreases the affinity.  
The transparent color cannot fill a white area. First fill the
white area with another color.

#### Spray
Click the [spray] button and move the mouse over the drawing area, holding down the left mouse button.

#### Pick color
Click the [pick] button and then on a point in the
image that you want to get the color of.  This will become the
active color.  Click on the pick button again to proceed
with other operations.

#### Line
Click the [line] button and then on the point in the image
where you want to start the shape area. Drag the edges of the
area to create the line.  The line can be moved by dragging the area
with the mouse.  
In the top menu choose the attributes of the line. 
To actually draw it, click on a point in the image.  
To deactivate it, uncheck the line button.
From the "Image->Create line" menu you can also create a line by entering the coordinates from the keyboard.

#### Shapes
As for the line. Click the [shapes] button.

#### Curved line
Click the [curved line] button.  You will be able to
click on 4 points of the image through which the bezier curve will pass.  
But only with the **fifth** click, it will actually be drawn.  
By clicking the curved line button again, any lines
not fixed with all clicks will be deleted.  
The real-time display of the bezier curve is not available.

#### Undo/Redo
In the top left selection box you can navigate through the history of
the last 8 changes.  To return to the left image, press the top left button [Redo].  
If a change is made from a previous image, the left image will no longer be available.

#### Zoom
##### Pixel matrix. 
Right-click on a point of the image, the pixel matrix will appear enlarged for the area in
the vicinity of the cursor. Clicking on a zoomed pixel with the
**right** mouse button, it will be replaced with the active color.  
Clicking with the left mouse button, you get the information on
the rgb color of the pixel.  
Remove the enlarged area with another right-click on the image.  
##### Double the pixels.
From the "Image->Quadruple the pixel" menu you can get a new image with the pixels doubled in width and height. The image can be enlarged several times without loss of quality, but it is not a zoom, but a new image.
To divide the pixels, use the "Image->Divide by 4" menu.
##### View.
The zoom of the entire image is obtained from the "Image->Zoom" menu, but
in view only.

#### Rotation 
This function requires that there is an active selection.  
Enter the degrees of rotation from 0 to 45 in the small box on the left and then click the right or left buttons to rotate.  
To rotate the selection by 90 degrees press the 90° buttons (right or left).  
To mirror the image go to the "Image->Mirror horizontally "or "Image->Mirror vertically" menu.

#### Sizes
From the" Image->Sizes" menu you can scale or resize the image based on manually entered values.  
You can also create a selection manually.

#### Effects
From the "Image->Effects" menu a window opens that allows you to apply effects to the image in the selection.  
If there is no selection, the effects are applied to the entire image.  
After each change, click the [Update] 
button to make the changes effective.  Click [OK] to apply the new image or [Cancel] to abort the changes.

#### Other effects
From the Image menu you can apply the listed effects to the selection.  
If there is no selection, the changes are applied to the entire image.  
The effects allow you to reduce the image to grayscale, invert the colors,
reduce to rgb (eliminates the minority channels and keeps only the prevalent one),
mirror the image.

#### Camera
A camera is a delimited area of ​​the image, whose dimensions
are defined by the user, and which can be saved in a file.  
This area can be moved freely with the mouse or with the
A Q S Z (or A W S X) keys.  
Save the portion of the image delimited by the camera from the "Camera->Save image" menu.  
Delete the camera from the" Camera->Delete camera " menu.

#### Saving
Images are saved in png format.  
To save in jpg, ico or bmp format, enter the extension to the file name
in the save window.  
#### video:
https://drive.google.com/file/d/1AIZnuqCW0IHnerJoBY9EjuAVHEOwcUgJ/view?usp=sharing

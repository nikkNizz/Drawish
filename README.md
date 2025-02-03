# Drawish  0.9.3 
Is a program for basic drawing.  
It also allows you to modify existing images.  
It also allows you to open pdf as images in linux


### PRECOMPILED PACKAGES:
The compiled program for Linux and Windows can be downloaded from the [releases](https://github.com/nikkNizz/Drawish/releases/tag/v0.9.3) page of this project.  
For **Windows**, download Drawish_version.zip.  
Extract the folder, and run the .exe file. Create a link to the executable for faster use.  

For **Debian/Linux** download and run the portable Appimage.

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

Or install and compile directly from **Qt Creator** on Windows, Mac, or Linux, without changing a single line of code!

#### USAGE
You can open images from the" File->Open" menu. You can also
paste an image from the clipboard ("Edit->Paste from clipboard") or from a file ("Edit->Paste from file")

The drawing options are visible on the left of the drawing area.
The color palette is also visible here.
The first color on the top left is the active color (default black). Change the active color:  
-by clicking on a quick color or on the + button  
-from the "Image->Colors" menu and entering the rgb values ​​manually  
-using the pick color on the image.  

When you select an option it will be active for the drawing area,
when you deselect it, it will no longer be active and any
pending operations will be deleted.

#### Command line
Launch the program from the command line.
You can also add the path of an image to open as an option.  
 `/path/Drawish`  
 `/path/Drawish /path/to/image`  

For Italian users: you can run Drawish in English by adding the "en" parameter.   
 `/path/Drawish en`  
 `/path/Drawish /path/to/image en`  

#### Open Pdf
It is possible to open pdf as images, dragging them onto the drawing area, or from the menu, or from the command line.   
In windows the quality is reduced.   
In Linux the image is real, thanks to the poppler-utils generally preinstalled. If they are not present, install them with your package manager.  
[video](https://drive.google.com/file/d/1Hy4OA9NZz2n9SRrA49Y8R8SDUAu0Y6TE/view?usp=sharing)

#### Selection
Click the [selection] button to select a
portion of the image. The selection will be detached from the image and
can be dragged with the mouse, and can also be resized by pulling the edges with the mouse.  
To copy the portion **without detaching it**, select it and,
before moving it, go to the "Edit->copy selection (no clipboard) " menu.  
You can also create a selection by entering the coordinates from the keyboard
from the "Sizes->Sizes->[Create selection]" menu.  
To move the selection finely with the keyboard use the A (left), Q or W (up), S (right), Z or X (down) keys[video](https://drive.google.com/file/d/1M-l_ubJsOdIr8C0IrRB_tgGHxzaZ9MT5/view?usp=sharing).  
To make the white area of ​​the selection transparent, check the box
from the "Edit->Transparent selection" Menu (already selected by default), **before** creating the selection.  
To select everything, use the "Edit->Select all" menu.  
To save the image to a file select: "Edit->Selection to file".  
To finish and pasting the selection, click on a point on the image.  

#### Merge images
From the "Edit->Paste from file" menu you can merge two images with three options. The second image can be put into a selection that can be moved to the desired point, or the second image can be automatically added to the right or below the first.
[Video](https://drive.google.com/file/d/1cbtphNuOdKlyNCEdMenNrpREKnVX_xbU/view?usp=sharing)

#### Merge selection
From the "Edit->Merge selection to image" menu you can merge the portion of the image below into the selection. 
A prompt allows you to choose between 3 blending ratios.
[video](https://drive.google.com/file/d/1GjlgFmv3VamLUOMfETiolQuof1ETXacL/view?usp=sharing)

#### Crop
From the "Edit->Selection to image (crop)" you can crop the image.

#### Freehand drawing
Click the [marker] button
and move the mouse over the drawing area, holding
down the left mouse button. You can draw
with the keyboard by selecting in the top menu how many pixels to draw
when you press the keys.  
Use the A (left), Q or W (up), S (right), Z or X (down) keys[video](https://drive.google.com/file/d/1M-l_ubJsOdIr8C0IrRB_tgGHxzaZ9MT5/view?usp=sharing).  
In the panel that appears above the drawing area you can
choose attributes. Do not use transparent color to draw.  
Use white color to erase.

#### Double pen
Click the [double pen] button.
Draw freehand with the mouse in 2 colors (black + the active color).

#### Text
Click the [text] button, and **click a point** in the drawing area. 
The text area will be displayed and can be moved as desired.
**The multiline text box and options are displayed at the top**.
Click the [DRAW] button to actually print the text, then move the text area 
to continue printing text. Or disable the function by clicking the left button again.
It is possible to paste text from the clipboard, as an image.
[video](https://drive.google.com/file/d/14wtJqm03hGNeiaqOLzPeB7In8m7ON_kk/view?usp=sharing)

#### Fill / Fill inside black
Click the [fill] button and click on the
point in the image where you want to replace a color with the
active color. The replacement occurs on the exact color that you clicked.  
To perform the replacement by color affinity, move the slider that
appears at the top.  
Increasing the value decreases the affinity.  
The transparent color cannot fill a white area. First fill the
white area with another color.
If the check box [Fill inside black] is selected, the entire area around the click, **included within a black line**, is filled in black. 
[video](https://drive.google.com/file/d/1KnkXdYvtXVhk3dniSauTWx8qtveAibHf/view?usp=sharing)

#### Spray
Click the [spray] button and move the mouse over the drawing area, holding down the left mouse button.

#### Pick color
Click the [picker] button and then on a point in the
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
From the "Image->Create line" menu **you can also create a line by entering the coordinates from the keyboard**.

#### Shapes
As for the line. Click the [shapes] button.
Shapes are: SQUARE, RECTANGLE, CIRCLE, ELLIPSE, TRIANGLE, ROUNDED RECT, STAR, ARROWS, CROSSES. 
Border width is defined by line width box. 
Many filling options are available from the choice box at the top right.
[video](https://drive.google.com/file/d/1t4i03DvqtAWYFMJfBzxKOIYCBJa3QD-V/view?usp=sharing)

#### Connected lines
Click the [connected lines] button.
After each click on the drawing area you will get connected lines. 
[video](https://drive.google.com/file/d/1KnkXdYvtXVhk3dniSauTWx8qtveAibHf/view?usp=sharing)

#### Curved line
Click the [curved line] button.  You will be able to
click on 4 points of the image through which the bezier curve will pass.    
The real-time display of the bezier curve is not available.

#### Undo/Redo
In the top left selection box you can navigate through the history of
the last 8 changes. You can also use the ctrl-Z key combination to go back.   
To return to the left image, press the top left button [Redo].  
If a change is made from a previous image, the left image will no longer be available.

#### Zoom
[video](https://drive.google.com/file/d/1exjhHi0g1fYensnA7L1kjqlH964XHt0q/view?usp=sharing)  
##### Pixel matrix. 
Right-click on a point of the image, the pixel matrix will appear enlarged for the area in
the vicinity of the cursor. Clicking on a zoomed pixel with the
**right** mouse button, it will be replaced with the active color.  
Clicking with the left mouse button, you get the information on
the rgb color of the pixel.  
Remove the enlarged area with another right-click on the image.  

##### Double the pixels or Increase by 10%.
If there is an active selection these functions applies to the selection, otherwise they applie to the entire image.
From the "Sizes->Quadruple the pixel" menu you can get a new image with the pixels doubled in width and height. 
The image can be enlarged several times without loss of quality, but it is not a zoom, but a new image.
In the same way, to divide the pixels, use the **"Sizes->Divide by 4"** menu.
From the "Sizes->Increment 10%" menu you can increase image size by 10% without loss.

##### Scaling
Increase the image size by scaling it from the menu "Sizes->Sizes" with loss of quality.  

##### View.
The zoom of the entire image is obtained from the "Sizes->Zoom" menu, but
in view only.

#### Rotation 
This function requires that there is an active selection.  
Enter the degrees of rotation from 0 to 45 in the small box on the left and then click the right or left buttons to rotate.  
To rotate the selection by 90 degrees press the 90° buttons (right or left).  
To combine 90 degree rotation and manual rotation, deselect and reselect.  
To mirror the image go to the "Image->Mirror horizontally "or "Image->Mirror vertically" menu.

#### Sizes
From the" Sizes->Sizes" menu you can scale or resize the image based on manually entered values.  
You can also create a selection manually.

#### Effects
From the "Image->Effects" menu a window opens that allows you to apply effects to the image in the selection.  
If there is no selection, the effects are applied to the entire image.  
After each change, click the [Update] button to make the changes effective, or select the "update automatically" 
checkbox if the function allows it.
Click [OK] to apply the new image or [Cancel] to abort the changes.

#### Stretch Area
From the "Image->Stretch area", you can open a window to distort the edges of the selected image.
[video](https://drive.google.com/file/d/1sh71bmC15u9uwQ95kfZ2oPRND_QK-o0W/view?usp=sharing)

#### Other effects
From the Image menu you can apply the listed effects to the selection.  
If there is no selection, the changes are applied to the entire image.  
The effects allow you to reduce the image to grayscale, invert the colors,
reduce to rgb (eliminates the minority channels and keeps only the prevalent one),
mirror the image, give sepia effect.

#### Camera
A camera is a delimited area of ​​the image, whose dimensions
are defined by the user, and which can be saved in a file.  
This area can be moved freely with the mouse or with the
A Q S Z (or A W S X) keys.  
Save the portion of the image delimited by the camera from the "Camera->Save image" menu.  
Delete the camera from the" Camera->Delete camera " menu.
You can save the framed image in base64 format to the clipboard from the "Camera->To base64" menu. 
[Video](https://drive.google.com/file/d/1cbtphNuOdKlyNCEdMenNrpREKnVX_xbU/view?usp=sharing)

#### Saving
Images are saved in png format.  
To save in jpg, ico or bmp format, enter the extension to the file name
in the save window.  
You can directly save the image to **PDF** from the "File->To PDF" menu.
For base64 format, see previous chapter.
#### video (old version):
[Introduction](https://drive.google.com/file/d/1P1VTq4DPX-Mfdl9mQaWzYBlodk4mRVRY/view?usp=sharing)  
[Simple project](https://drive.google.com/file/d/1qYL_KbZe9HY8I8-WeP1Zw0g9_m4A9Qy3/view?usp=sharing)

Icons freely modified from "[Breeze Icons](https://develop.kde.org/frameworks/breeze-icons/) copyright KDE and licenced under the GNU LGPL version 3 or later".  
# Drawish  2.5 
Is a program for basic drawing.  
It also allows you to modify existing images, open pdf as images in Linux, draw using the keyboard, use rich text, save portions framed by a "camera" and much more.  
[See features](#features)  

![Image](https://github.com/nikkNizz/Drawish/blob/main/png/DRAWISCREEN1.png)  


### PRECOMPILED PACKAGES:
The compiled program for Linux and Windows can be downloaded from the [releases](https://github.com/nikkNizz/Drawish/releases) page of this project.  
Once the program is open you can create a quick link: from the [File -> Desktop shortcut] menu, a menu item is created in Linux or a desktop shortcut in Windows.

### COMPILE FROM SOURCE:
Download and unzip the project.  
Give the following commands from the terminal (instructions for Debian derivatives):

`sudo apt update`  
`sudo apt install g++`  
`sudo apt install cmake`  
`sudo apt install qt6-base-dev`  
`cd /path/to/Drawish_project`  
`cmake .`  
`make`  
*optional*  
`sudo make install` 

When there are changes, replace only the modified files with the newer ones and simply run the `make` command. The process will be very fast.

Or install and compile directly from **Qt Creator** on Windows, Mac, or Linux, without changing a single line of code!

On some Linux systems, you may need to download the following libraries in order to compile:  
apt install \
    libfontconfig1-dev \  
    libfreetype-dev \  
    libx11-dev \  
    libx11-xcb-dev \  
    libxcb-cursor-dev \  
    libxcb-glx0-dev \  
    libxcb-icccm4-dev \  
    libxcb-image0-dev \  
    libxcb-keysyms1-dev \  
    libxcb-randr0-dev \  
    libxcb-render-util0-dev \  
    libxcb-shape0-dev \  
    libxcb-shm0-dev \  
    libxcb-sync-dev \  
    libxcb-util-dev \  
    libxcb-xfixes0-dev \  
    libxcb-xinerama0-dev \  
    libxcb-xkb-dev \  
    libxcb1-dev \  
    libxext-dev \  
    libxfixes-dev \  
    libxi-dev \  
    libxkbcommon-dev \  
    libxkbcommon-x11-dev \  
    libxrender-dev  

#### Command line
If you launch the program from the command line, you can also add the path of an image.  
 `/path/Drawish_xxxx.Appimage`  or  
 `/path/Drawish_xxxx.Appimage /path/to/image`  

For Italian users: you can run Drawish in English by adding the "en" parameter.   
 `/path/Drawish_xxxx.Appimage en`  or  
 `/path/Drawish_xxxx.Appimage /path/to/image en`  

#### USAGE
You can start drawing or you can open an image.  
Open images from the" File->Open" menu. You can also drag a file on the drawing area, 
paste an image from the clipboard ("Edit->Paste from clipboard") or from a file ("Edit->Paste from file").  
Some **settings**, such as pen size or text options, are automatically restored from the previous session.  

The drawing options are visible on the left of the drawing area.
The color palette is also visible here.
The first color on the top left is the active color (default black). Change the active color:  
-by clicking on a quick color or on the + button  
-from the "Image->Colors" menu and entering the rgb values ​​manually  
-using the pick color on the image.  
-From saved colors.  

When you select an option it will be active for the drawing area,
when you deselect it, it will no longer be active and any
pending operations will be deleted.


#### Saving
Images are saved in png format.  
To save in jpg, ico, bmp, ppm, xbm or xpm format, choose the extension from the "format" box that appears.  
You can directly save the image to **PDF** from the "File->To PDF" menu. PDFs are saved in "<user>/Drawish_Data" or in home folder.  
For base64 format, see "Camera" chapter.  
You can set a **default folder** for the file picker: "Menu ->Default path for file picker".

#### Serialize
From the "Edit->Serialize" menu a window opens from which you can choose operations to perform on all the images contained in a folder.  
Choose the folder, check the operations of interest and enter any parameters.  
The **original images will not be overwritten** as a new folder will be created for the modified files.  
The operations will be performed in the order in which they are listed.

#### Undo/Redo
In the top left selection box you can navigate through the history of
the last 25 changes. You can also use the **ctrl-Z** key combination to go back.   
To return to the left image, press the top left button or **ctrl-R** [Redo].  
![image](https://github.com/nikkNizz/Drawish/blob/main/png/REDO.png)  
If a change is made from a previous image, the left image will no longer be available.  
[Video](https://drive.google.com/file/d/1jMbZppeGw3zGTbC74FTZjxg4T8YVSmn-/view?usp=sharing)

#### Restore point
You can save a restore point from the [image->Set as restore point] menu (or F1 key). Any previous points will be deleted.  
Go back to the saved point at any time from the [Image->Restore] menu (F12).  
Cancel the operation with ctrl-Z or undo


#### Open Pdf
It is possible to open pdf as images, dragging them onto the drawing area, or from the menu, or from the command line.   
In windows the quality is reduced.   
In Linux the image is real, thanks to the poppler-utils generally preinstalled. If they are not present, install them with your package manager.  
[video](https://drive.google.com/file/d/1S440ixRX7CHNSXD62h23SyD1GWCo3j0U/view?usp=sharing)

#### Selection
Click the [selection] button to select a portion of the image. The selection will be detached from the image and
can be dragged with the mouse, and can also be resized by pulling the edges with the mouse.  
Hold the mouse **near the inside** of the edges to resize the selection.  
To copy the portion **without detaching it** (stamp), select it and, before moving it, go to the "Edit->copy selection (no clipboard) " menu, or press **+** on your keyboard or ctrl + .[video](https://drive.google.com/file/d/1t025EHseLYV5s-1YnYc8WolSRT5IJJB2/view?usp=sharing).  
You can also create a selection by entering the coordinates from the keyboard from the "Sizes->Sizes->[Create selection]" menu.  
To move the selection finely with the keyboard use the A (left), Q or W (up), S (right), Z or X (down) keys.  
To make the white area of ​​the selection transparent, check the box
from the "Edit->Transparent selection" Menu (selected by default).  
To select everything, use the "Edit->Select all" menu.  
To save the image to a file select: "Edit->Selection to file".  
To finish and pasting the selection, click on a point on the image.  

#### Freehand selection
Click the [Freehand selection] to start making a freehand selection with the mouse.  
See also [Fill inside black](https://github.com/nikkNizz/Drawish/blob/main/README.md#fill--fill-inside-black)

#### Elliptical selection and other shapes
You can transform a shape into a selection.
Draw a shape as usual, then press the button to the right of the fill selection box to convert the shape into a selection.  
[video](https://drive.google.com/file/d/1OmfOUle8yHQT_zte3cJ2GencHQ7gQVGY/view?usp=sharing)


#### Merge selection
From the "Edit->Merge selection to image" menu you can merge the portion of the image below into the selection. 
A prompt allows you to choose between 3 blending ratios.
[video](https://drive.google.com/file/d/15H6inAFvbu6iay29mfOW9ALRBSV0VWz6/view?usp=sharing)

#### Merge images
From the "Edit->Paste from file" menu you can merge two images with three options. The second image can be put into a selection that can be moved to the desired point, or the second image can be automatically added to the right or below the first.

#### Crop
From the "Edit->Set selection as image (crop)" you can crop the image. 
> On very large images, you can crop them for faster processing and then paste them again using the 'Paste over file' function. The prompt will remind you the coordinates of the last crop.
> 
#### Paste over file
From the "Edit->Paste over file" menu, you can paste the current image into a file at the specified coordinates.  
If a crop has been previously made, the crop coordinates are proposed at the prompt.

#### Freehand drawing
Click the [pen] button and move the mouse over the drawing area, holding down the left mouse button.  
From the "edit" menu you can enable **"Mouse correction"** to avoid drawing every little mouse movement.  
You can draw with the keyboard by selecting in the top menu how many pixels to draw when you press the keys ( 1 or 5).  
Keys: (Q = top-left)  (W = top)  (E = top-right)   (A = left)  (S = right)  (< = bottom-left)  (Z = down)  (X = bottom-right). 
[video](https://drive.google.com/file/d/1Z6KETxX53N0E_YGQgtdDkzLk7GZgHE8c/view?usp=sharing)  
In the panel that appears above the drawing area you can choose attributes. Do not use transparent color to draw (only fill).  
You can use a semi-transparent pen.  
Use white color to erase.  
You can draw with a round, square, linear, or stylus tip.  
You can draw with a random color pen.  
You can use the **"fusion"** pen to draw with a color obtained by merging the pixels crossed by the pen.  

[video](https://drive.google.com/file/d/15WTKDzYWJppr3aST-LWUw6-EzQZaqXTG/view?usp=sharing)  

#### Color eraser
Click the [pen] button. In the panel that appears above the drawing area select "Erase by color similaraty defined in fill".  
When the cursor moves over the active color, it will be replaced by the preferred color.  
To replace it with white, reset the preferred color with the appropriate button.  
To perform the similarity replacement, enter the "Fill" function, move the "similarity" bar and then return to the "Pen" function.  
[video](https://drive.google.com/file/d/1NbB-NSNq2GasxtETI4yEmdrg-jJ0YJwJ/view?usp=sharing)

#### Text
Click the [text] button, and **click a point** in the drawing area. 
The text area will be displayed and can be moved as desired.
**The multiline text box and options are displayed at the top**.
Click the [DRAW] button to actually print the text, then move the text area 
to continue printing text. Or disable the function by clicking the left button again.
You can open an **RTF editor** to write formatted text. This text can be saved to a file and can be directly transposed onto the image via a screenshot. After clicking the [text] button, click the [RTF] button to open the editor. Click [Screenshot and close] button to transpose the text into a selection in the image. In the editor you can save texts and open them.  
It is possible to paste text from the clipboard, as an image.  
[video](https://drive.google.com/file/d/1GrowsX4R_jyyqmt8AIxP22UySknR3SwT/view?usp=sharing)

#### Tracer
Click [tracer] to activate it.  
The tracer can be used if you use Drawish in demonstrations. Allows you to draw a highlight line that automatically disappears.

#### Fill / Fill inside black
Click the [fill] button and click on the point in the image where you want to replace a color with the active color. The replacement occurs on the exact color that you clicked.  
To perform the replacement by color affinity, move the slider that appears at the top.  
Increasing the value decreases the affinity.  
It is possible to fill with semi transparency of the active color with the switch at the top right.[video](https://drive.google.com/file/d/1GynDndzshitoALd6C1AE_JqcklSExQDD/view?usp=sharing)  
If the check box [Fill inside black] is selected, the entire area around the click, **included within a black line**, is filled in black. 
[video](https://drive.google.com/file/d/1yWAFPJW5sjcYRTpn0P7AlbdcRmab0lex/view?usp=sharing)  

#### Spray
Click the [spray] button and move the mouse over the drawing area, holding down the left mouse button.  
The size of the spray is defined by the size of the pen.

#### Pick color
Click the [pick] button and then on a point in the
image that you want to get the color of. This will become the
active color.  
If the [color balance] check box is checked, you can balance the colors of the image based on the clicked color and the active color. 
For example, if you want to balance the white, you will have to set white as the active color and click on a color in the image that should be white, but is not.
[video](https://drive.google.com/file/d/1M87q7asCIgoTUkzosI1L7IYRID0MK-JZ/view?usp=sharing)  
Click on the pick button again to proceed with other operations.

#### Line / Simple arrow / Connected lines
Click the [Line] button. In the selection box next to it, select a line or a simple arrow or connected lines.  
In the top menu choose the attributes of the line.  
To draw lines and arrows use the mouse on the image with the left button pressed.  
To get connected lines click on the points to connect.  
From the "Image->Create line" menu (or ALT + L) **you can also create a line by entering the coordinates from the keyboard**.  
The coordinates can be expressed as the **start and end points** of the line, or as the **start point, angle and length!**
The angles are in the order shown in the image:  
![image](https://drive.google.com/uc?export=download&id=1GYvPe_yhgc-y3qFFtwj_QH60IaK1Wj6F)  
[video](https://drive.google.com/file/d/1z3qcvtxO0pj5NvT5CZHCLOQmpNo4UeXj/view?usp=sharing)  
[video](https://drive.google.com/file/d/1hArIyCshNnlM2Vb6YnhPleDx2v98CpSU/view?usp=sharing)  


#### Shapes
Click the [shapes] button, or select a shape from the selection box.  
The shape can be moved by dragging the area with the mouse or from keyboard (A W S Z keys).  
Shapes are: SQUARE, RECTANGLE, CIRCLE, ELLIPSE, TRIANGLE, ROUNDED RECT, ROUNDED SQUARE, STAR, ARROWS, SOLID, CROSSES (for simple arrow see "Line / Simple arrow / Connected lines" chapter).  
Border width is defined by line width box. 
Many filling options are available from the choice box at the top right.  

#### Shapes by keyboard
You can create rectangles, squares, circles and ellipses by entering data from the keyboard from the "Image-->Create Shapes" menu (or ALT + H).  
**Enter the center** as the shape coordinate.  
You can draw the center of these shapes or not.  


#### Curved line
Click the [curved line] button. You will be able to click on **6 points** of the image through which the bezier curve will pass.    
By moving the handles, you can create any complex curved shape.
To confirm the drawing, click a point on the image.
[video](https://drive.google.com/file/d/1sRTAQ8MWXeTsjSYYcyFzLdEWoPF3-TAZ/view?usp=sharing)

#### Connected curves
Click the [connected curves] button.  
Click **four** points of the image to draw the first curve, then click on **two** points to create more curves.  
[video](https://drive.google.com/file/d/1HAuOjxhacs0KzekiKnL53k9hrXIMokUr/view?usp=sharing)

#### Color management
From the [Image->Colors] menu you can perform additional operations on colors.  
*If transparent selection is active, the transparent color is treated as black*.  
You can create a color by entering rgb values ​​from the keyboard, create gradients and replace pixels based on complex options.  
You can also get the hexadecimal value of the color and the average color of the image.  
[video](https://drive.google.com/file/d/1PJ-7u52q-H2AVHOg521qymgIlQxkHiOt/view?usp=sharing)  
[video](https://drive.google.com/file/d/1A_yMq7SHWH1n-Yu3GsRULe9LDZ2Vc3gg/view?usp=sharing)

#### Favorite colors
From the menu [Image-> Set as favorite color] the active color is saved in an image and made available in the color palette as an additional color.  
The favorite color is also used for the "color eraser" function. The little button next to the favorite color resets it.  
The colors are saved in the file "<user>/Drawish_Data/drawish_saved_colors.png".  
From this file is very simple to pick a color. From the [Image -> Pick from favorites] menu click on a color. This will become the active color.  
[video](https://drive.google.com/file/d/1SB7doXszCFuWpwYHjILtjKa0UpjAFG5z/view?usp=sharing)

#### Color balance
See Pick color.  
[video](https://drive.google.com/file/d/1M87q7asCIgoTUkzosI1L7IYRID0MK-JZ/view?usp=sharing)  

#### Charts
Select the [Charts] menu and choose a chart type. Enter positive values ​​(one per line) in the box that opens.  

#### Zoom
From the "Zoom" menu choose a predefined zoom level or a custom level.  
For quick zoom levels you can press the 1 to 6 keys on your keyboard!

##### Pixel matrix. 
Right click on the image->Show Grid: the pixel matrix will appear enlarged for the area in the vicinity of the cursor.  
Clicking on a zoomed pixel with the **right** mouse button, it will be replaced with the active color (or draw with the active color by holding down the right mouse button).  
Clicking with the **left** mouse button, to get information about the rgb color of the pixel and possibly set it as the active color.  
Remove the enlarged area with another right-click on the image.  

##### Double the pixels.
If there is an active selection these functions applies to the selection, otherwise they applie to the entire image.
From the "Sizes->Quadruple the pixel" menu (or ALT + Q) you can increase the image with the pixels doubled in width and height. 
The image can be enlarged several times without loss of quality, but it is not a zoom, but an image modification.
In the same way, to divide the pixels, use the **"Sizes->Divide by 4"** menu (ALT + D).

#### Rotation  
Enter the degrees of rotation in the small box on the left and then click the right or left buttons to rotate.  
If you don't know how many degrees to rotate the image, start the automatic rotation by clicking the small button with the stopwatch and then click again to stop it in the desired position.
To mirror the image (or selection) go to the "Image->Mirror horizontally "or "Image->Mirror vertically" menu.  
[video](https://drive.google.com/file/d/1M5SMg7tclV4CMdQZlDANKdyLmEfvo7yT/view?usp=sharing)

#### Effects
From the "Image->Effects" menu a window opens that allows you to apply effects to the image in the selection.  
*If transparent selection is active, the transparent color is treated as black*.  
If there is no selection, the effects are applied to the entire image.  
After each change, click the [Update] button to make the changes effective, or select the "update automatically" 
checkbox if the function allows it.
Click [OK] to apply the new image or [Cancel] to abort the changes.  
> When an image is particularly faded, and is represented by very close intensity values, instead of increasing the contrast, try to perform "histogram equalization".   
> I implemented the function following the instructions on [Wikipedia](https://en.wikipedia.org/wiki/Histogram_equalization).

#### Stretch Area
From the "Image->Stretch area", you can open a window to distort the edges of the selected image.  
You can perform a curved distortion by selecting the appropriate box.  
[video](https://drive.google.com/file/d/1Bu-tI1thtWPNiWgkfMz513TjMefXXL8-/view?usp=sharing)


#### Other effects
From the Image menu you can apply the listed effects to the selection.  
If there is no selection, the changes are applied to the entire image.  
The effects allow you to reduce the image to grayscale, invert the colors,
reduce to rgb (eliminates the minority channels and keeps only the prevalent one),
mirror the image, give sepia effect, find the outlines of objects. 
[video](https://drive.google.com/file/d/1pdrpBKdsCYndaHA34EnvAZ_ENh_BBu5D/view?usp=sharing)  

#### Sizes
From the" Sizes->Sizes" (Alt-S) menu you can scale or resize the image based on manually entered values.  
It is also possible to adapt the image to a new **form factor**. [video](https://drive.google.com/file/d/1LXlnjbYWZuqWUPemhjpaHtxBgadD2UWA/view?usp=sharing)  
You can also create a **selection manually**.  


#### Camera
A camera is a delimited area of ​​the image, whose dimensions
are defined by the user, and which can be saved in a file.  
This area can be moved freely with the mouse or with the
A Q S Z (or A W S X) keys.  
Save the portion of the image delimited by the camera from the "Camera->Save image" menu.  
Delete the camera from the" Camera->Delete camera " menu.
You can save the framed image in base64 format to the clipboard from the "Camera->To base64" menu. 
[Video](https://drive.google.com/file/d/1nAk2Pm1FmSXZ3rD-pW398qp9vDhU7IQS/view?usp=sharing)

#### Window
From the "Sizes->Window size" menu you can set the size of the Drawish window.  
From the "About->Capture me (to clipboard)" menu or with the Alt+K combination you can save the screenshot of the window to the clipboard.  

#### Key combination
key combination  
  
|button | button |  Function  |
--------|--------|----------------------------------------------|
CTRL   |    Z   |   UNDO  
CTRL   |    R   |   REDO  
\+     |        |   COPY SELECTION (NO CLIPBOARD)
CTRL   |    +   |   COPY SELECTION (NO CLIPBOARD)  
ALT    |    A   |   SELECT ALL  
ALT    |    C   |   OPEN COLORS WINDOW  
ALT    |    D   |   DIVIDE BY 4  
ALT    |    E   |   OPEN EFFECTS WINDOW  
ALT    |    G   |   GITHUB / GUIDE  
ALT    |    H   |   CREATE SHAPE  
ALT    |    K   |   SCREENSHOT (CLIPBOARD)  
ALT    |    L   |   CREATE LINE  
ALT    |    Q   |   QUADRUPLE PIXELS  
ALT    |    T   |   OPEN STRETCH AREA  
ALT    |    S   |   OPEN SIZE WINDOW  
F1     |        |   SET RESTORE POINT  
F12    |        |   RESTORE  
1      |        |   ZOOM 100 %  
2      |        |   ZOOM 200 %  
3      |        |   ZOOM 300 %  
4      |        |   ZOOM 400 %  
5      |        |   ZOOM 50 %  
6      |        |   ZOOM 30 %  

MOVE SELECTION OR CAMERA  
      A, S, W, Z = LEFT,RIGHT,TOP, BOTTOM  
  
WRITE WITH PEN  
       Q = TOP-LEFT  
       W = TOP  
       E = TOP-RIGHT  
       A = LEFT  
       S = RIGHT  
       < = BOTTOM-LEFT  
       Z = BOTTOM  
       X = BOTTOM-RIGHT  

#### Features
Here are some of Drawish's unique features (The most common functions are omitted):  

🟢️ color replacement with conditions  
🟢️ large number of shapes with many types of filling   
🟢️ manual selection creation  
🟢️ keyboard line for point A and B  
🟢️ keyboard line for point A - angle - length  
🟢️ keyboard drawing: Right left up down and diagonals  
🟢️ rich text editor  
🟢️ fill inside black (useful for eliminating an area or background quickly)  
🟢️ paste from file with automatic union at the bottom or right or leave in the selection  
🟢️ file saving camera  
🟢️ get code base64 (from Camera)  
🟢️ complex curves  
🔵️ merging selection and image with 3 levels of strength  
🔵️ shapes to selection  
🔵️ paste over file    
🔵️ manual or continuous selection rotation  
🔵️ curve-like stretching  
🔵️ gradients  
🔵️ keyboard color definition  
🔵️ tracer  
🔵️ semi-transparent pen    
🔵️ keyboard shapes with center displayed  
🔵️ fill with semi transparency    
🔵️ reduce to rgb    
🔵️ create charts  
🔵️ open pdf as images and save images as pdf  
🔵️ Interface for Batch functions  
🔵️ quick restore point  
🔵️ quick web links  
🔵️ object outline  
🟣️ mouse correction  
🟣️ effects  
🟣️ average color  
🟣️ fill for similarity  
🟣️ eraser for color by similarity  
🟣️ grid and pixel color replacement with right clicks  
🟣️ multicolor pen  
🟣️ set the window size  
🟣️ screenshot of the window  
🟣️ fusion pen    
🟣️ favorite colors  
🟣️ copy selection to the program, not only to clipboard  
🟣️ increase image or selection size without loss (in addition to scaling by pulling edges)  



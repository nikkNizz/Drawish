<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>790</width>
    <height>450</height>
   </rect>
  </property>
  <property name="acceptDrops">
   <bool>true</bool>
  </property>
  <property name="windowTitle">
   <string>Drawish</string>
  </property>
  <property name="windowIcon">
   <iconset resource="Drawish.qrc">
    <normaloff>:/res/draw.png</normaloff>:/res/draw.png</iconset>
  </property>
  <property name="toolTip">
   <string/>
  </property>
  <property name="dockOptions">
   <set>QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks</set>
  </property>
  <widget class="QWidget" name="centralwidget">
   <widget class="QScrollArea" name="scrollArea">
    <property name="geometry">
     <rect>
      <x>80</x>
      <y>30</y>
      <width>451</width>
      <height>331</height>
     </rect>
    </property>
    <property name="widgetResizable">
     <bool>true</bool>
    </property>
    <widget class="QWidget" name="scrollAreaWidgetContents">
     <property name="geometry">
      <rect>
       <x>0</x>
       <y>0</y>
       <width>449</width>
       <height>329</height>
      </rect>
     </property>
    </widget>
   </widget>
   <widget class="QPushButton" name="undoButton">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>26</y>
      <width>40</width>
      <height>41</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Redo</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/redo.png</normaloff>:/res/redo.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>false</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="selectionAreaButton">
    <property name="geometry">
     <rect>
      <x>40</x>
      <y>28</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Select area</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/selez.png</normaloff>:/res/selez.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="checked">
     <bool>false</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="penButton">
    <property name="geometry">
     <rect>
      <x>40</x>
      <y>60</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Pen</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/penna.png</normaloff>:/res/penna.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="fillButton">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>93</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Fill</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/fill.png</normaloff>:/res/fill.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="drawTextButton">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>60</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Type text</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/text.png</normaloff>:/res/text.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="pickerButton">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>126</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Color picker</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/pipet.png</normaloff>:/res/pipet.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="lineButton">
    <property name="geometry">
     <rect>
      <x>40</x>
      <y>126</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Line</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/line.png</normaloff>:/res/line.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="shapeButton">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>190</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Shapes</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/shapes.png</normaloff>:/res/shapes.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="sprayButton">
    <property name="geometry">
     <rect>
      <x>40</x>
      <y>93</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Spray</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/spray.png</normaloff>:/res/spray.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="curveButton">
    <property name="geometry">
     <rect>
      <x>40</x>
      <y>190</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Curve line</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/curve.png</normaloff>:/res/curve.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="blackButton">
    <property name="geometry">
     <rect>
      <x>32</x>
      <y>299</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="flat">
     <bool>false</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="whiteButton">
    <property name="geometry">
     <rect>
      <x>55</x>
      <y>299</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>170</red>
          <green>170</green>
          <blue>170</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>170</red>
          <green>170</green>
          <blue>170</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="colorActiveButton">
    <property name="geometry">
     <rect>
      <x>9</x>
      <y>297</y>
      <width>21</width>
      <height>23</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="toolTip">
     <string>Active color</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="flat">
     <bool>false</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="greyButton">
    <property name="geometry">
     <rect>
      <x>9</x>
      <y>326</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>154</red>
          <green>153</green>
          <blue>150</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>231</red>
          <green>230</green>
          <blue>225</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>192</red>
          <green>191</green>
          <blue>187</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>77</red>
          <green>77</green>
          <blue>75</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>103</red>
          <green>102</green>
          <blue>100</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>154</red>
          <green>153</green>
          <blue>150</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>204</red>
          <green>204</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>154</red>
          <green>153</green>
          <blue>150</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>231</red>
          <green>230</green>
          <blue>225</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>192</red>
          <green>191</green>
          <blue>187</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>77</red>
          <green>77</green>
          <blue>75</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>103</red>
          <green>102</green>
          <blue>100</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>154</red>
          <green>153</green>
          <blue>150</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="redButton">
    <property name="geometry">
     <rect>
      <x>32</x>
      <y>326</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>63</green>
          <blue>63</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>170</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>63</green>
          <blue>63</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>170</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="greenButton">
    <property name="geometry">
     <rect>
      <x>55</x>
      <y>326</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>102</red>
          <green>255</green>
          <blue>112</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>69</red>
          <green>232</green>
          <blue>79</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>24</red>
          <green>139</green>
          <blue>32</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="BrightText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>145</red>
          <green>232</green>
          <blue>151</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ToolTipBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>220</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ToolTipText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="PlaceholderText">
        <brush brushstyle="SolidPattern">
         <color alpha="127">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>102</red>
          <green>255</green>
          <blue>112</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>69</red>
          <green>232</green>
          <blue>79</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>24</red>
          <green>139</green>
          <blue>32</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="BrightText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>145</red>
          <green>232</green>
          <blue>151</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ToolTipBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>220</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ToolTipText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="PlaceholderText">
        <brush brushstyle="SolidPattern">
         <color alpha="127">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="WindowText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>102</red>
          <green>255</green>
          <blue>112</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>69</red>
          <green>232</green>
          <blue>79</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>24</red>
          <green>139</green>
          <blue>32</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Text">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="BrightText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ButtonText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Base">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>36</red>
          <green>209</green>
          <blue>47</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ToolTipBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>220</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="ToolTipText">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="PlaceholderText">
        <brush brushstyle="SolidPattern">
         <color alpha="127">
          <red>18</red>
          <green>105</green>
          <blue>24</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="blueButton">
    <property name="geometry">
     <rect>
      <x>9</x>
      <y>353</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>26</red>
          <green>95</green>
          <blue>180</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>52</red>
          <green>143</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>39</red>
          <green>119</green>
          <blue>217</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>13</red>
          <green>47</green>
          <blue>90</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>17</red>
          <green>63</green>
          <blue>120</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>26</red>
          <green>95</green>
          <blue>180</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>140</red>
          <green>175</green>
          <blue>217</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>26</red>
          <green>95</green>
          <blue>180</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>52</red>
          <green>143</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>39</red>
          <green>119</green>
          <blue>217</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>13</red>
          <green>47</green>
          <blue>90</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>17</red>
          <green>63</green>
          <blue>120</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>26</red>
          <green>95</green>
          <blue>180</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="yellowButton">
    <property name="geometry">
     <rect>
      <x>32</x>
      <y>353</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>246</red>
          <green>211</green>
          <blue>45</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>239</green>
          <blue>161</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>250</red>
          <green>225</green>
          <blue>103</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>123</red>
          <green>105</green>
          <blue>22</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>164</red>
          <green>141</green>
          <blue>30</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>246</red>
          <green>211</green>
          <blue>45</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>250</red>
          <green>233</green>
          <blue>150</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>246</red>
          <green>211</green>
          <blue>45</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>239</green>
          <blue>161</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>250</red>
          <green>225</green>
          <blue>103</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>123</red>
          <green>105</green>
          <blue>22</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>164</red>
          <green>141</green>
          <blue>30</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>246</red>
          <green>211</green>
          <blue>45</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="magentaButton">
    <property name="geometry">
     <rect>
      <x>55</x>
      <y>353</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>127</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>63</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>0</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>170</red>
          <green>0</green>
          <blue>170</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>127</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>127</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>63</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>0</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>170</red>
          <green>0</green>
          <blue>170</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>0</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="cyanButton">
    <property name="geometry">
     <rect>
      <x>9</x>
      <y>380</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>63</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>170</green>
          <blue>170</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>255</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Light">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>127</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>63</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>127</green>
          <blue>127</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>170</green>
          <blue>170</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>255</green>
          <blue>255</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="text">
     <string/>
    </property>
   </widget>
   <widget class="QPushButton" name="transparentButton">
    <property name="geometry">
     <rect>
      <x>32</x>
      <y>380</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>238</red>
          <green>238</green>
          <blue>236</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>111</red>
          <green>110</green>
          <blue>109</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>148</red>
          <green>147</green>
          <blue>145</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>238</red>
          <green>238</green>
          <blue>236</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>238</red>
          <green>238</green>
          <blue>236</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>111</red>
          <green>110</green>
          <blue>109</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>148</red>
          <green>147</green>
          <blue>145</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="toolTip">
     <string>Transparent</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/trasp.png</normaloff>:/res/trasp.png</iconset>
    </property>
   </widget>
   <widget class="QPushButton" name="addColorButton">
    <property name="geometry">
     <rect>
      <x>55</x>
      <y>380</y>
      <width>20</width>
      <height>21</height>
     </rect>
    </property>
    <property name="palette">
     <palette>
      <active>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>238</red>
          <green>238</green>
          <blue>236</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>111</red>
          <green>110</green>
          <blue>109</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>148</red>
          <green>147</green>
          <blue>145</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>0</red>
          <green>0</green>
          <blue>0</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>238</red>
          <green>238</green>
          <blue>236</blue>
         </color>
        </brush>
       </colorrole>
      </active>
      <inactive>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>202</red>
          <green>202</green>
          <blue>202</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>159</red>
          <green>159</green>
          <blue>159</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>184</red>
          <green>184</green>
          <blue>184</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>239</red>
          <green>239</green>
          <blue>239</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>118</red>
          <green>118</green>
          <blue>118</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </inactive>
      <disabled>
       <colorrole role="Button">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Midlight">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>238</red>
          <green>238</green>
          <blue>236</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Dark">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>111</red>
          <green>110</green>
          <blue>109</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Mid">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>148</red>
          <green>147</green>
          <blue>145</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Window">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>222</red>
          <green>221</green>
          <blue>218</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="Shadow">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>177</red>
          <green>177</green>
          <blue>177</blue>
         </color>
        </brush>
       </colorrole>
       <colorrole role="AlternateBase">
        <brush brushstyle="SolidPattern">
         <color alpha="255">
          <red>247</red>
          <green>247</green>
          <blue>247</blue>
         </color>
        </brush>
       </colorrole>
      </disabled>
     </palette>
    </property>
    <property name="font">
     <font>
      <pointsize>8</pointsize>
      <bold>true</bold>
     </font>
    </property>
    <property name="text">
     <string>+</string>
    </property>
   </widget>
   <widget class="QWidget" name="textOptionsWidget" native="true">
    <property name="geometry">
     <rect>
      <x>80</x>
      <y>0</y>
      <width>501</width>
      <height>31</height>
     </rect>
    </property>
    <widget class="QPushButton" name="boldButton">
     <property name="geometry">
      <rect>
       <x>0</x>
       <y>3</y>
       <width>21</width>
       <height>24</height>
      </rect>
     </property>
     <property name="font">
      <font>
       <bold>true</bold>
      </font>
     </property>
     <property name="text">
      <string>B</string>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
    </widget>
    <widget class="QPushButton" name="italicButton">
     <property name="geometry">
      <rect>
       <x>25</x>
       <y>3</y>
       <width>21</width>
       <height>24</height>
      </rect>
     </property>
     <property name="font">
      <font>
       <italic>true</italic>
       <bold>false</bold>
      </font>
     </property>
     <property name="text">
      <string>I</string>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
    </widget>
    <widget class="QPushButton" name="underlineButton">
     <property name="geometry">
      <rect>
       <x>50</x>
       <y>3</y>
       <width>21</width>
       <height>24</height>
      </rect>
     </property>
     <property name="font">
      <font>
       <bold>false</bold>
       <underline>true</underline>
      </font>
     </property>
     <property name="text">
      <string>U</string>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
    </widget>
    <widget class="QFontComboBox" name="fontComboBox">
     <property name="geometry">
      <rect>
       <x>80</x>
       <y>3</y>
       <width>121</width>
       <height>24</height>
      </rect>
     </property>
    </widget>
    <widget class="QLineEdit" name="sizeLine">
     <property name="geometry">
      <rect>
       <x>240</x>
       <y>3</y>
       <width>31</width>
       <height>24</height>
      </rect>
     </property>
     <property name="text">
      <string>16</string>
     </property>
    </widget>
    <widget class="QLabel" name="label">
     <property name="geometry">
      <rect>
       <x>210</x>
       <y>6</y>
       <width>31</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Size</string>
     </property>
    </widget>
    <widget class="QPushButton" name="confirmTextButton">
     <property name="geometry">
      <rect>
       <x>435</x>
       <y>3</y>
       <width>61</width>
       <height>24</height>
      </rect>
     </property>
     <property name="text">
      <string>DRAW</string>
     </property>
    </widget>
    <widget class="QPlainTextEdit" name="textEdit">
     <property name="geometry">
      <rect>
       <x>280</x>
       <y>0</y>
       <width>151</width>
       <height>31</height>
      </rect>
     </property>
     <property name="placeholderText">
      <string>Text</string>
     </property>
    </widget>
   </widget>
   <widget class="QWidget" name="lineOptionWidget" native="true">
    <property name="geometry">
     <rect>
      <x>190</x>
      <y>50</y>
      <width>551</width>
      <height>31</height>
     </rect>
    </property>
    <widget class="QLabel" name="label_2">
     <property name="geometry">
      <rect>
       <x>0</x>
       <y>5</y>
       <width>41</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Width</string>
     </property>
    </widget>
    <widget class="QSpinBox" name="lineWidthBox">
     <property name="geometry">
      <rect>
       <x>40</x>
       <y>0</y>
       <width>41</width>
       <height>25</height>
      </rect>
     </property>
     <property name="minimum">
      <number>1</number>
     </property>
     <property name="maximum">
      <number>40</number>
     </property>
     <property name="value">
      <number>6</number>
     </property>
    </widget>
    <widget class="QPushButton" name="flatcapButton">
     <property name="geometry">
      <rect>
       <x>90</x>
       <y>0</y>
       <width>25</width>
       <height>26</height>
      </rect>
     </property>
     <property name="toolTip">
      <string>Flat cap</string>
     </property>
     <property name="text">
      <string/>
     </property>
     <property name="icon">
      <iconset resource="Drawish.qrc">
       <normaloff>:/res/flatCap.png</normaloff>:/res/flatCap.png</iconset>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
     <property name="checked">
      <bool>false</bool>
     </property>
    </widget>
    <widget class="QPushButton" name="roundcapButton">
     <property name="geometry">
      <rect>
       <x>120</x>
       <y>0</y>
       <width>25</width>
       <height>26</height>
      </rect>
     </property>
     <property name="toolTip">
      <string>Round cap</string>
     </property>
     <property name="text">
      <string/>
     </property>
     <property name="icon">
      <iconset resource="Drawish.qrc">
       <normaloff>:/res/roundCap.png</normaloff>:/res/roundCap.png</iconset>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
     <property name="checked">
      <bool>true</bool>
     </property>
    </widget>
    <widget class="QRadioButton" name="radioButton">
     <property name="geometry">
      <rect>
       <x>210</x>
       <y>0</y>
       <width>101</width>
       <height>21</height>
      </rect>
     </property>
     <property name="font">
      <font>
       <pointsize>9</pointsize>
      </font>
     </property>
     <property name="text">
      <string>Keyboard 1px</string>
     </property>
     <property name="checked">
      <bool>true</bool>
     </property>
    </widget>
    <widget class="QRadioButton" name="radioButton_2">
     <property name="geometry">
      <rect>
       <x>310</x>
       <y>0</y>
       <width>51</width>
       <height>21</height>
      </rect>
     </property>
     <property name="font">
      <font>
       <pointsize>10</pointsize>
      </font>
     </property>
     <property name="text">
      <string> 5px</string>
     </property>
    </widget>
    <widget class="QComboBox" name="shapesCombo">
     <property name="geometry">
      <rect>
       <x>370</x>
       <y>0</y>
       <width>91</width>
       <height>24</height>
      </rect>
     </property>
     <item>
      <property name="text">
       <string>Square ☐</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Rectangle ☐</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Circle ○</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Ellipse ○</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Triangle△</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Rounded rectangle</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Star  ☆</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow up ⇑</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow Right ⇒</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow Down ⇓</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow left ⇐</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow ⇖</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow ⇗</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow ⇘</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Arrow ⇙</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Coross +</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Cross X</string>
      </property>
     </item>
    </widget>
    <widget class="QPushButton" name="markerButton">
     <property name="geometry">
      <rect>
       <x>150</x>
       <y>0</y>
       <width>25</width>
       <height>26</height>
      </rect>
     </property>
     <property name="toolTip">
      <string>Marker</string>
     </property>
     <property name="text">
      <string/>
     </property>
     <property name="icon">
      <iconset resource="Drawish.qrc">
       <normaloff>:/res/trasp.png</normaloff>:/res/trasp.png</iconset>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
    </widget>
    <widget class="QComboBox" name="comboBox">
     <property name="geometry">
      <rect>
       <x>470</x>
       <y>0</y>
       <width>81</width>
       <height>24</height>
      </rect>
     </property>
     <item>
      <property name="text">
       <string>No Fill</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Fill solid</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense1</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense2</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense3</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense4</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense5</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense6</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Dense7</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Horizontal lines</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Vertical lines</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Cross lines</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Diagon lines B</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Diagon lines F</string>
      </property>
     </item>
     <item>
      <property name="text">
       <string>Diagon cross</string>
      </property>
     </item>
    </widget>
    <widget class="QPushButton" name="nibButton">
     <property name="geometry">
      <rect>
       <x>180</x>
       <y>0</y>
       <width>25</width>
       <height>26</height>
      </rect>
     </property>
     <property name="toolTip">
      <string>Nib cap</string>
     </property>
     <property name="text">
      <string>\</string>
     </property>
     <property name="checkable">
      <bool>true</bool>
     </property>
     <property name="checked">
      <bool>false</bool>
     </property>
    </widget>
   </widget>
   <widget class="QWidget" name="similaritywidget" native="true">
    <property name="geometry">
     <rect>
      <x>329</x>
      <y>100</y>
      <width>461</width>
      <height>31</height>
     </rect>
    </property>
    <widget class="QLabel" name="label_3">
     <property name="geometry">
      <rect>
       <x>10</x>
       <y>10</y>
       <width>61</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Similarity</string>
     </property>
    </widget>
    <widget class="QSlider" name="similaritySlider">
     <property name="geometry">
      <rect>
       <x>70</x>
       <y>10</y>
       <width>200</width>
       <height>16</height>
      </rect>
     </property>
     <property name="toolTip">
      <string>Fill for similarity (0 fill the exact color)</string>
     </property>
     <property name="maximum">
      <number>99</number>
     </property>
     <property name="orientation">
      <enum>Qt::Horizontal</enum>
     </property>
    </widget>
    <widget class="QLabel" name="similarityLabel">
     <property name="geometry">
      <rect>
       <x>280</x>
       <y>8</y>
       <width>31</width>
       <height>20</height>
      </rect>
     </property>
     <property name="text">
      <string>0</string>
     </property>
    </widget>
    <widget class="QCheckBox" name="fillInsideBlack_check">
     <property name="geometry">
      <rect>
       <x>310</x>
       <y>8</y>
       <width>141</width>
       <height>22</height>
      </rect>
     </property>
     <property name="font">
      <font>
       <pointsize>8</pointsize>
      </font>
     </property>
     <property name="text">
      <string>Fill inside black</string>
     </property>
    </widget>
   </widget>
   <widget class="QWidget" name="infoWidget" native="true">
    <property name="geometry">
     <rect>
      <x>80</x>
      <y>370</y>
      <width>591</width>
      <height>41</height>
     </rect>
    </property>
    <widget class="QLabel" name="label_4">
     <property name="geometry">
      <rect>
       <x>0</x>
       <y>10</y>
       <width>51</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Mouse</string>
     </property>
    </widget>
    <widget class="QLabel" name="selX_label">
     <property name="geometry">
      <rect>
       <x>180</x>
       <y>10</y>
       <width>31</width>
       <height>16</height>
      </rect>
     </property>
     <property name="frameShape">
      <enum>QFrame::Box</enum>
     </property>
     <property name="text">
      <string>x</string>
     </property>
    </widget>
    <widget class="QLabel" name="mouseY_label">
     <property name="geometry">
      <rect>
       <x>80</x>
       <y>10</y>
       <width>31</width>
       <height>16</height>
      </rect>
     </property>
     <property name="frameShape">
      <enum>QFrame::Box</enum>
     </property>
     <property name="text">
      <string>y</string>
     </property>
    </widget>
    <widget class="QLabel" name="label_5">
     <property name="geometry">
      <rect>
       <x>120</x>
       <y>10</y>
       <width>60</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Selection</string>
     </property>
    </widget>
    <widget class="QLabel" name="mouseX_label">
     <property name="geometry">
      <rect>
       <x>50</x>
       <y>10</y>
       <width>31</width>
       <height>16</height>
      </rect>
     </property>
     <property name="frameShape">
      <enum>QFrame::Box</enum>
     </property>
     <property name="text">
      <string>x</string>
     </property>
    </widget>
    <widget class="QLabel" name="selY_label">
     <property name="geometry">
      <rect>
       <x>210</x>
       <y>10</y>
       <width>31</width>
       <height>16</height>
      </rect>
     </property>
     <property name="frameShape">
      <enum>QFrame::Box</enum>
     </property>
     <property name="text">
      <string>y</string>
     </property>
    </widget>
    <widget class="QLabel" name="label_6">
     <property name="geometry">
      <rect>
       <x>250</x>
       <y>10</y>
       <width>51</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Image</string>
     </property>
    </widget>
    <widget class="QLabel" name="imageW_label">
     <property name="geometry">
      <rect>
       <x>300</x>
       <y>10</y>
       <width>41</width>
       <height>16</height>
      </rect>
     </property>
     <property name="frameShape">
      <enum>QFrame::Box</enum>
     </property>
     <property name="text">
      <string>w</string>
     </property>
    </widget>
    <widget class="QLabel" name="imageH_label">
     <property name="geometry">
      <rect>
       <x>350</x>
       <y>10</y>
       <width>41</width>
       <height>16</height>
      </rect>
     </property>
     <property name="frameShape">
      <enum>QFrame::Box</enum>
     </property>
     <property name="text">
      <string>h</string>
     </property>
    </widget>
    <widget class="QLabel" name="rgbLabel">
     <property name="geometry">
      <rect>
       <x>410</x>
       <y>10</y>
       <width>141</width>
       <height>16</height>
      </rect>
     </property>
     <property name="text">
      <string>Rgb 0,0,0</string>
     </property>
    </widget>
   </widget>
   <widget class="QPushButton" name="rotateLeftButton">
    <property name="geometry">
     <rect>
      <x>0</x>
      <y>235</y>
      <width>21</width>
      <height>24</height>
     </rect>
    </property>
    <property name="toolTip">
     <string>Rotate left</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/rotateLeft.png</normaloff>:/res/rotateLeft.png</iconset>
    </property>
   </widget>
   <widget class="QPushButton" name="rotateRightButton">
    <property name="geometry">
     <rect>
      <x>60</x>
      <y>235</y>
      <width>21</width>
      <height>24</height>
     </rect>
    </property>
    <property name="toolTip">
     <string>Rotate right</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/rotateRight.png</normaloff>:/res/rotateRight.png</iconset>
    </property>
   </widget>
   <widget class="QSpinBox" name="RotatioAngleSpin">
    <property name="geometry">
     <rect>
      <x>20</x>
      <y>234</y>
      <width>41</width>
      <height>25</height>
     </rect>
    </property>
    <property name="toolTip">
     <string>Rotation angle</string>
    </property>
    <property name="minimum">
     <number>1</number>
    </property>
    <property name="maximum">
     <number>45</number>
    </property>
    <property name="value">
     <number>25</number>
    </property>
   </widget>
   <widget class="QPushButton" name="deg90left">
    <property name="geometry">
     <rect>
      <x>0</x>
      <y>261</y>
      <width>31</width>
      <height>20</height>
     </rect>
    </property>
    <property name="toolTip">
     <string>Rotate left</string>
    </property>
    <property name="text">
     <string>90°</string>
    </property>
   </widget>
   <widget class="QPushButton" name="deg90right">
    <property name="geometry">
     <rect>
      <x>50</x>
      <y>261</y>
      <width>31</width>
      <height>20</height>
     </rect>
    </property>
    <property name="toolTip">
     <string>Rotate right</string>
    </property>
    <property name="text">
     <string>90°</string>
    </property>
   </widget>
   <widget class="QComboBox" name="historyCombo">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>5</y>
      <width>71</width>
      <height>21</height>
     </rect>
    </property>
    <property name="toolTip">
     <string>Remove last...</string>
    </property>
   </widget>
   <widget class="QPushButton" name="conn_Curve">
    <property name="geometry">
     <rect>
      <x>40</x>
      <y>158</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Connected curves</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/conCurve.png</normaloff>:/res/conCurve.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QPushButton" name="connLine">
    <property name="geometry">
     <rect>
      <x>4</x>
      <y>158</y>
      <width>40</width>
      <height>36</height>
     </rect>
    </property>
    <property name="cursor">
     <cursorShape>PointingHandCursor</cursorShape>
    </property>
    <property name="toolTip">
     <string>Connected lines</string>
    </property>
    <property name="text">
     <string/>
    </property>
    <property name="icon">
     <iconset resource="Drawish.qrc">
      <normaloff>:/res/lineConn.png</normaloff>:/res/lineConn.png</iconset>
    </property>
    <property name="iconSize">
     <size>
      <width>28</width>
      <height>30</height>
     </size>
    </property>
    <property name="checkable">
     <bool>true</bool>
    </property>
    <property name="flat">
     <bool>true</bool>
    </property>
   </widget>
   <widget class="QWidget" name="widgetPick" native="true">
    <property name="geometry">
     <rect>
      <x>560</x>
      <y>160</y>
      <width>171</width>
      <height>31</height>
     </rect>
    </property>
    <widget class="QCheckBox" name="colorBalanceCheck">
     <property name="geometry">
      <rect>
       <x>10</x>
       <y>0</y>
       <width>161</width>
       <height>22</height>
      </rect>
     </property>
     <property name="text">
      <string>Color balance</string>
     </property>
    </widget>
   </widget>
  </widget>
  <widget class="QStatusBar" name="statusbar"/>
  <widget class="QMenuBar" name="menubar">
   <property name="geometry">
    <rect>
     <x>0</x>
     <y>0</y>
     <width>790</width>
     <height>21</height>
    </rect>
   </property>
   <widget class="QMenu" name="menuFile">
    <property name="title">
     <string>File</string>
    </property>
    <widget class="QMenu" name="menuRecent">
     <property name="title">
      <string>Recent</string>
     </property>
    </widget>
    <widget class="QMenu" name="menuLinks">
     <property name="title">
      <string>Links</string>
     </property>
    </widget>
    <addaction name="actionNew"/>
    <addaction name="actionOpen"/>
    <addaction name="actionSave"/>
    <addaction name="actionSave_as"/>
    <addaction name="actionTo_Pdf"/>
    <addaction name="menuRecent"/>
    <addaction name="menuLinks"/>
    <addaction name="actionAdd_link"/>
    <addaction name="actionClose"/>
   </widget>
   <widget class="QMenu" name="menuEdit">
    <property name="title">
     <string>Edit</string>
    </property>
    <widget class="QMenu" name="menuPaste_from_file">
     <property name="title">
      <string>Paste from file</string>
     </property>
     <addaction name="actionAdd_right"/>
     <addaction name="actionAdd_bottom"/>
     <addaction name="actionAdd_as_selection"/>
    </widget>
    <addaction name="actionSelect_all"/>
    <addaction name="actionCopy_selection_no_clipboard"/>
    <addaction name="actionCopy"/>
    <addaction name="actionSet"/>
    <addaction name="actionPaste_from_clipboard"/>
    <addaction name="menuPaste_from_file"/>
    <addaction name="actionCopy_selection_to_file"/>
    <addaction name="actionMerge_selection_to_image"/>
    <addaction name="actionTransparent_selection"/>
   </widget>
   <widget class="QMenu" name="menuEffects">
    <property name="title">
     <string>Image</string>
    </property>
    <addaction name="actionEffects"/>
    <addaction name="actionColors"/>
    <addaction name="actionMirror"/>
    <addaction name="actionMirror_vertically"/>
    <addaction name="actionStretch_area"/>
    <addaction name="actionTo_greyscale"/>
    <addaction name="actionTo_sepia"/>
    <addaction name="actionReduce_to_RGB"/>
    <addaction name="actionInvert_colors"/>
    <addaction name="actionCreate_Line"/>
    <addaction name="actionSet_as_restore_point"/>
    <addaction name="actionRestore"/>
   </widget>
   <widget class="QMenu" name="menuCamera">
    <property name="title">
     <string>Camera</string>
    </property>
    <addaction name="actionCreate"/>
    <addaction name="actionSave_image"/>
    <addaction name="actionDelete_camera"/>
    <addaction name="actionbase64"/>
   </widget>
   <widget class="QMenu" name="menuAbout">
    <property name="title">
     <string>About</string>
    </property>
    <addaction name="actionAbout"/>
    <addaction name="actionGithub"/>
   </widget>
   <widget class="QMenu" name="menuSizes">
    <property name="title">
     <string>Sizes</string>
    </property>
    <addaction name="actionSizes_2"/>
    <addaction name="actionQuadruple_the_pixels_2"/>
    <addaction name="actionDivide_by_5"/>
    <addaction name="actionIncrement_10"/>
    <addaction name="actionZoom_2"/>
   </widget>
   <widget class="QMenu" name="menuGraphics">
    <property name="title">
     <string>Charts</string>
    </property>
    <addaction name="actionHorizontal_bars"/>
    <addaction name="actionVertical_bars"/>
    <addaction name="actionPie"/>
   </widget>
   <addaction name="menuFile"/>
   <addaction name="menuEdit"/>
   <addaction name="menuSizes"/>
   <addaction name="menuEffects"/>
   <addaction name="menuCamera"/>
   <addaction name="menuGraphics"/>
   <addaction name="menuAbout"/>
  </widget>
  <action name="actionNew">
   <property name="text">
    <string>New</string>
   </property>
  </action>
  <action name="actionOpen">
   <property name="text">
    <string>Open</string>
   </property>
  </action>
  <action name="actionSave">
   <property name="text">
    <string>Save</string>
   </property>
  </action>
  <action name="actionClose">
   <property name="text">
    <string>Close</string>
   </property>
  </action>
  <action name="actionSave_as">
   <property name="text">
    <string>Save as</string>
   </property>
  </action>
  <action name="actionSelect_all">
   <property name="text">
    <string>Select all</string>
   </property>
  </action>
  <action name="actionCopy">
   <property name="text">
    <string>Copy selection</string>
   </property>
  </action>
  <action name="actionPaste_from_clipboard">
   <property name="text">
    <string>Paste from clipboard</string>
   </property>
  </action>
  <action name="actionCopy_selection_to_file">
   <property name="text">
    <string>Selection to file</string>
   </property>
  </action>
  <action name="actionTransparent_selection">
   <property name="checkable">
    <bool>true</bool>
   </property>
   <property name="checked">
    <bool>true</bool>
   </property>
   <property name="text">
    <string>Transparent selection</string>
   </property>
  </action>
  <action name="actionCopy_selection_no_clipboard">
   <property name="text">
    <string>Copy selection (no clipboard)</string>
   </property>
  </action>
  <action name="actionEffects">
   <property name="text">
    <string>Effects</string>
   </property>
  </action>
  <action name="actionMirror">
   <property name="text">
    <string>Mirror horizontally</string>
   </property>
  </action>
  <action name="actionMirror_vertically">
   <property name="text">
    <string>Mirror vertically</string>
   </property>
  </action>
  <action name="actionTo_greyscale">
   <property name="text">
    <string>To greyscale</string>
   </property>
  </action>
  <action name="actionInvert_colors">
   <property name="text">
    <string>Invert colors</string>
   </property>
  </action>
  <action name="actionCreate">
   <property name="text">
    <string>Create</string>
   </property>
  </action>
  <action name="actionSave_image">
   <property name="text">
    <string>Save image</string>
   </property>
  </action>
  <action name="actionDelete_camera">
   <property name="text">
    <string>Delete camera</string>
   </property>
  </action>
  <action name="actionColors">
   <property name="text">
    <string>Colors</string>
   </property>
  </action>
  <action name="actionReduce_to_RGB">
   <property name="text">
    <string>Reduce to RGB</string>
   </property>
  </action>
  <action name="actionAbout">
   <property name="text">
    <string>About</string>
   </property>
  </action>
  <action name="actionCreate_Line">
   <property name="text">
    <string>Create Line</string>
   </property>
  </action>
  <action name="actionGithub">
   <property name="text">
    <string>Github</string>
   </property>
  </action>
  <action name="actionMerge_selection_to_image">
   <property name="checkable">
    <bool>false</bool>
   </property>
   <property name="text">
    <string>Merge selection to image</string>
   </property>
  </action>
  <action name="actionbase64">
   <property name="text">
    <string>To base64</string>
   </property>
  </action>
  <action name="actionTo_Pdf">
   <property name="text">
    <string>To Pdf</string>
   </property>
  </action>
  <action name="actionAdd_right">
   <property name="text">
    <string>Add right</string>
   </property>
  </action>
  <action name="actionAdd_bottom">
   <property name="text">
    <string>Add bottom</string>
   </property>
  </action>
  <action name="actionAdd_as_selection">
   <property name="text">
    <string>Add as selection</string>
   </property>
  </action>
  <action name="actionStretch_area">
   <property name="text">
    <string>Stretch area</string>
   </property>
  </action>
  <action name="actionTo_sepia">
   <property name="text">
    <string>To sepia</string>
   </property>
  </action>
  <action name="actionSet">
   <property name="text">
    <string>Set selection as image (crop)</string>
   </property>
  </action>
  <action name="actionSizes_2">
   <property name="text">
    <string>Sizes</string>
   </property>
  </action>
  <action name="actionQuadruple_the_pixels_2">
   <property name="text">
    <string>Quadruple the pixels</string>
   </property>
  </action>
  <action name="actionDivide_by_5">
   <property name="text">
    <string>Divide by 4</string>
   </property>
  </action>
  <action name="actionIncrement_10">
   <property name="text">
    <string>Increment 10%</string>
   </property>
  </action>
  <action name="actionZoom_2">
   <property name="text">
    <string>Zoom</string>
   </property>
  </action>
  <action name="actionAdd_link">
   <property name="text">
    <string>Add remove link</string>
   </property>
  </action>
  <action name="actionHorizontal_bars">
   <property name="text">
    <string>Horizontal bars</string>
   </property>
  </action>
  <action name="actionVertical_bars">
   <property name="text">
    <string>Vertical bars</string>
   </property>
  </action>
  <action name="actionPie">
   <property name="text">
    <string>Pie</string>
   </property>
  </action>
  <action name="actionSet_as_restore_point">
   <property name="text">
    <string>Set as restore point</string>
   </property>
  </action>
  <action name="actionRestore">
   <property name="text">
    <string>Restore</string>
   </property>
  </action>
 </widget>
 <resources>
  <include location="Drawish.qrc"/>
 </resources>
 <connections/>
</ui>

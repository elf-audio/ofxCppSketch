# ofxCppSketch

This is an openFrameworks wrapper for [cppsketch](https://github.com/elf-audio/ofxCppSketch.git), which allows you to auto-recompile just your ofApp without recompiling the whole project. The point of this is so that you can more easily sketch small ideas without having to wait too long to see the results of tweaking numbers.

This only works on the mac for now, only tested with Xcode 11.

[![Video of how to set up ofxCppSketch](https://img.youtube.com/vi/UPIhMJ8_HOU/0.jpg)](https://www.youtube.com/watch?v=UPIhMJ8_HOU)

## HOW TO USE
1. Use project generator to add ofxCppSketch or drag its folder into an empty project
2. Change your main.cpp to `#include "ofxCppSketch.h"`
3. change the line 	
`ofRunApp(new ofApp());`
to
`ofRunApp(new ofxCppSketch("ofApp", __FILE__));`

4. Go into project build settings, find "Dead code stripping" - change it to NO

Now, every time you save ofApp.h or ofApp.cpp whilst your app is running, it will recompile in the background - errors are displayed in the xcode console if compilation fails.

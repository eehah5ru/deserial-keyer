#pragma once

#include "string"

#include "ofMain.h"
#include "ofxChromaKey.h"
#include "ofxGreenscreen.h"
#include "ofxAVFVideoPlayer.h"
#include "ofxUI.h"

#include "videoPlayers.h"



class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit ();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // gui event handler
    void guiEvent(ofxUIEventArgs &e);


private:
    //
    // attrs
    //
    ofxUISuperCanvas*   _gui;
//    ofxChromaKey        bluescreen;
    ofxGreenscreen      _greenscreen;
    ofVideoGrabber      _grabber;
    videoPlayers        _videoPlayers;
    bool                _needToSwapMovies;
    
    bool                _bkgColorPickerEnabled;
    bool                _mouseVisible;

    ofPoint _dragStart;
    ofPoint _greenPixelPos;

    //
    // methods
    //
    
    // init grabber
    void initGrabber ();
    
    void initGui ();
    
    void updateBgColorFormGuiBgColor ();
    
    void updateGuiBgColorFormBgColor ();
    
    int getLeftTopX ();
    
    int getLeftTopY ();
    
    int getBottomRightX ();
    
    int getBottomRightY ();
};

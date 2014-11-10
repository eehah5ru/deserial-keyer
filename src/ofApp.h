#pragma once

#include "string"

#include "ofMain.h"
#include "ofxChromaKey.h"
//#include "ofxGreenscreen.h"
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
    
//        string getRandomFileFromDirectory ();
//    
//        void setupDirectory (string path);
//    
//    
//        void setupMovies ();
//    
//        void loadRandomCurrentMovie ();
//    
//        void loadRandomNextMovie ();
//    
//        void loadRandomMovie (ofxAVFVideoPlayer* ptr);
//    
//        void swapMovies ();
//    
//        ofxAVFVideoPlayer* getCurrentBkgMovie ();
    
        // init grabber
        void initGrabber ();
    
//        ofxGeenscreen
//        ofxGreenscreen      bluescreen;
            ofVideoGrabber      grabber;
        ofxChromaKey        bluescreen;

    
        videoPlayers        _videoPlayers;
    
//        ofDirectory         dir;
    
//        ofVideoPlayer 		bkgMovie;
//        ofxAVFVideoPlayer   bkgMovieA;
//        ofxAVFVideoPlayer   bkgMovieB;
//    
//        ofxAVFVideoPlayer*   currentBkgMovie;
//    
//        ofxAVFVideoPlayer*   nextBkgMovie;
//    
        bool             needToSwapMovies;
		
    
};

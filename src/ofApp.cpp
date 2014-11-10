#include "ofApp.h"


#define GRAB_WIDTH  1024
#define GRAB_HEIGHT 576




//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("deSerialKeyer");
    ofBackground(0,0,0);
    ofSetFrameRate(60);
    
    ofSetVerticalSync(true);
    

    initGrabber();
    
    
    
    _videoPlayers.initPlayers("bkgs_1024x576");

//    grabber.initGrabber(GRAB_WIDTH, GRAB_HEIGHT);
    grabber.initGrabber(640, 480);
    bluescreen.setPixels(grabber.getPixelsRef());
}


void ofApp::exit () {
    _videoPlayers.free();
}

//--------------------------------------------------------------
void ofApp::update() {
    grabber.update();
    
    if(grabber.isFrameNew())
        bluescreen.setPixels(grabber.getPixelsRef());
    

    _videoPlayers.current()->update();
    

    // prepare new movie
    if (ofInRange(_videoPlayers.current()->getPosition(), 0.8, 0.9)) {
        needToSwapMovies = true;
    }
    
    if (ofInRange(_videoPlayers.current()->getPosition(), 0.99, 1)) {
        if (needToSwapMovies) {
            _videoPlayers.changePlayer();
            needToSwapMovies = false;
        }
        
        _videoPlayers.current()->update();
    }
    
    if (_videoPlayers.current()->isLoaded() && !_videoPlayers.current()->isPlaying()) {
        _videoPlayers.current()->play();
        _videoPlayers.current()->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    int x0 = (ofGetWidth() - GRAB_WIDTH) / 2;
    int y0 = (ofGetHeight() - GRAB_HEIGHT) / 2;
    
    _videoPlayers.current()->draw(x0, y0);


    bluescreen.draw(x0, y0, GRAB_WIDTH, GRAB_HEIGHT, false);

//    grabber.draw(0, 0, 160, 120);
//    bluescreen.drawBgColor();
    
    //
    // info
    //
    ofSetHexColor(0xffffff);
    char reportStr[1024];
    sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %f (press: +/-)\nfps: %f", bluescreen.getThreshold(), ofGetFrameRate());
//    sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nfps: %f", ofGetFrameRate());
    ofDrawBitmapString(reportStr, x0, y0 + GRAB_HEIGHT + 20);
    
//    float scale =(float)ofGetWidth()/ (float)GRAB_WIDTH;
//    ofScale(scale, scale);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_UP)
        bluescreen.setThreshold(bluescreen.getThreshold()+5);
    if(key == OF_KEY_DOWN)
        bluescreen.setThreshold(bluescreen.getThreshold()-5);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' ')
        bluescreen.learnBgColor(grabber.getPixelsRef());
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::initGrabber () {
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = grabber.listDevices();
    
    //
    // show info and
    // set DV-VCR as video input device
    //
    bool found = false;
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id  << ": '" << devices[i].deviceName << "'";
        if( devices[i].bAvailable ){
            // set device to DV-VCR
            if (string("DV-VCR").compare(devices[i].deviceName) == 0) {
                grabber.setDeviceID(i);
                found = true;
            }
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
        
        // debug formats
//        vector<ofVideoFormat> formats = devices[i].formats;
//        for (auto j : formats) {
//            cout << "\twidth: " << j.width << " height: " << j.height << endl;
//        }
        
//        grabber.setDesiredFrameRate(25);
    }
    
    if (!found) {
        grabber.setDeviceID(0);
    }
}

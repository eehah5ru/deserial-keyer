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

    _greenscreen.setPixels(_grabber.getPixelsRef());
    
    initGui();
}


void ofApp::exit () {
    _videoPlayers.free();
    _gui->saveSettings("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update() {
    _grabber.update();
    
    if(_grabber.isFrameNew())
        _greenscreen.setPixels(_grabber.getPixelsRef());
    

    _videoPlayers.current()->update();
    
    updateGuiBgColorFormBgColor();
    
    //
    // update cursor state
    //
//    if (_gui->isVisible()) {
//        ofGetWindowPtr()->showCursor();
//    } else {
//        ofGetWindowPtr()->hideCursor();
//    }

    // prepare new movie
    if (ofInRange(_videoPlayers.current()->getPosition(), 0.8, 0.9)) {
        _needToSwapMovies = true;
    }
    
    if (ofInRange(_videoPlayers.current()->getPosition(), 0.99, 1)) {
        if (_needToSwapMovies) {
            _videoPlayers.changePlayer();
            _needToSwapMovies = false;
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

    
    _videoPlayers.current()->draw(getLeftTopX(), getLeftTopY());


    _greenscreen.draw(getLeftTopX(), getLeftTopY(), GRAB_WIDTH, GRAB_HEIGHT, false);
    


//    grabber.draw(0, 0, 160, 120);
//    bluescreen.drawBgColor();
    
    //
    // info
    //
//    ofSetHexColor(0xffffff);
//    char reportStr[1024];
//    sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %f (press: +/-)\nfps: %f", bluescreen.getThreshold(), ofGetFrameRate());
//    sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nfps: %f", ofGetFrameRate());
//    ofDrawBitmapString(reportStr, x0, y0 + GRAB_HEIGHT + 20);
    
//    float scale =(float)ofGetWidth()/ (float)GRAB_WIDTH;
//    ofScale(scale, scale);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    if(key == OF_KEY_UP)
//        bluescreen.setThreshold(bluescreen.getThreshold()+5);
//    if(key == OF_KEY_DOWN)
//        bluescreen.setThreshold(bluescreen.getThreshold()-5);
    
    if (key == 't') {
        _gui->toggleVisible();
    }
    
    if (key == 'm') {
        if (_mouseVisible) {
            ofGetWindowPtr()->hideCursor();
        }
        else {
            ofGetWindowPtr()->showCursor();
        }
        
        _mouseVisible = !_mouseVisible;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // exit if gui is not visible
    if (!_gui->isVisible()) {
        return;
    }
    
    // ok. lets do it
    if(key == ' ') {
        _greenscreen.learnBgColor(_grabber.getPixelsRef());
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // exit if gui is not visible
    if (!_gui->isVisible()) {
        return;
    }
    
    if (!_bkgColorPickerEnabled) {
        return;
    }
    
    // debug
//    cout << "mouse dragged. dragStart.x=" << ofToString(dragStart.x) << " dragStart.y=" << ofToString(dragStart.y) << endl;
    
    // ok. lets do it
    if(x == _dragStart.x || y == _dragStart.y) {
        return;
    }
    
    
    _greenscreen.learnBgColor(_grabber.getPixelsRef(), _dragStart.x, _dragStart.y, x - _dragStart.x, y - _dragStart.y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    _dragStart.set(x, y);
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

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if(name == "base mask strength")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        _greenscreen.strengthBaseMask = slider->getScaledValue();
    }
    else if(name == "chroma mask strength")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        _greenscreen.strengthChromaMask = slider->getScaledValue();
    }
    else if(name == "green spill strength")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        _greenscreen.strengthGreenSpill = slider->getScaledValue();
    }
    else if(name == "base mask black")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        _greenscreen.clipBlackBaseMask = rslider->getScaledValue();
    }
    else if(name == "base mask white")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        _greenscreen.clipWhiteBaseMask = rslider->getScaledValue();
    }
    else if(name == "detail mask black")
    {
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        _greenscreen.clipBlackDetailMask = rslider->getScaledValue();
    }
    else if(name == "detail mask white")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        _greenscreen.clipWhiteDetailMask = slider->getScaledValue();
    }
    else if(name == "end mask black")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        _greenscreen.clipBlackEndMask = slider->getScaledValue();
    }
    else if(name == "end mask white")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        _greenscreen.clipWhiteEndMask = slider->getScaledValue();
    }
}


void ofApp::initGrabber () {
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = _grabber.listDevices();
    
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
                _grabber.setDeviceID(i);
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
        _grabber.setDeviceID(0);
    }
    
    _grabber.initGrabber(GRAB_WIDTH, GRAB_HEIGHT);
}


void ofApp::initGui () {
    _gui = new ofxUISuperCanvas("SETTINGS", 400, 0, 200, 600);
    
    //    gui1 = new ofxUISuperCanvas("Images", 400, 0, 200, 600);
    
    //gui->addTitle("SETTINGS");
    _gui->addToggle("detail mask", &_greenscreen.doDetailMask);
    _gui->addToggle("base mask", &_greenscreen.doBaseMask);
    _gui->addToggle("chroma mask", &_greenscreen.doChromaMask);
    _gui->addToggle("greenspill", &_greenscreen.doGreenSpill);
    
    
    //    gui->addColorPicker("key color", bgColor);
    _gui->addSlider("base mask strength", 0.0, 1.f, _greenscreen.strengthBaseMask);
    _gui->addSlider("chroma mask strength", 0.0, 1.f, _greenscreen.strengthChromaMask);
    _gui->addSlider("green spill strength", 0.0, 1.f, _greenscreen.strengthGreenSpill);
    
    
    //gui->addTitle("CLIPPING");
    _gui->addSlider("base mask black", 0.0, 1.f, _greenscreen.clipBlackBaseMask);
    _gui->addSlider("base mask white", 0.0, 1.f, _greenscreen.clipWhiteBaseMask);
    _gui->addSlider("detail mask black", 0.0, 1.f, _greenscreen.clipBlackDetailMask);
    _gui->addSlider("detail mask white", 0.0, 1.f, _greenscreen.clipWhiteDetailMask);
    _gui->addSlider("end mask black", 0.0, 1.f, _greenscreen.clipBlackEndMask);
    _gui->addSlider("end mask white", 0.0, 1.f, _greenscreen.clipWhiteEndMask);
    _gui->addToggle("Enable bkg color picker", &_bkgColorPickerEnabled);
    _gui->addLabel("bkg color");
    _gui->addFPS();
    
    _gui->autoSizeToFitWidgets();
    
    ofAddListener(_gui->newGUIEvent,this,&ofApp::guiEvent);
    
    _gui->loadSettings("settings.xml");
    
    updateBgColorFormGuiBgColor();
    
    _gui->setVisible(false);
//    ofGetWindowPtr()->hideCursor();
}


void ofApp::updateBgColorFormGuiBgColor () {
    ofxUILabel* label = static_cast<ofxUILabel*>(_gui->getWidget("bkg color"));
    
    
    _greenscreen.setBgColor(ofColor(ofFromString<int>(label->getLabel())));
}

void ofApp::updateGuiBgColorFormBgColor () {
    ofxUILabel* label = static_cast<ofxUILabel*>(_gui->getWidget("bkg color"));
    
    label->setColorBack(_greenscreen.getBgColor());
    label->setColorFill(_greenscreen.getBgColor());
    label->setLabel(ofToString(_greenscreen.getBgColor().getHex()));
    
}


int ofApp::getLeftTopX () {
    return (ofGetWidth() - GRAB_WIDTH) / 2;

}


int ofApp::getLeftTopY () {
    return (ofGetHeight() - GRAB_HEIGHT) / 2;
}


int ofApp::getBottomRightX () {
    return getLeftTopX() + GRAB_WIDTH;
}

int ofApp::getBottomRightY () {
    return getLeftTopY() + GRAB_HEIGHT;
}



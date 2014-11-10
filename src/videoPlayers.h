//
//  videoPlayers.h
//  deSerialKeyer
//
//  Created by nicola spesivcev on 08.11.14.
//
//

#include "ofMain.h"
#include "ofxAVFVideoPlayer.h"

#ifndef deSerialKeyer_videoPlayers_h
#define deSerialKeyer_videoPlayers_h

class videoPlayers {
    vector<ofxAVFVideoPlayer *> _players;
    
    int _curPlayerIndex = 0;
    
public:
    videoPlayers ();
    ~videoPlayers ();
    
    void initPlayers (string path);
    
    void free ();
    
    ofxAVFVideoPlayer* current ();
    
    void changePlayer ();
    
    // interface to ofApp
    void update ();
    
    void draw (int x, int y, int w, int h);
    
private:
    void addVideoPlayer(const string& path);
    
};

#endif

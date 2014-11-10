//
//  videoPlayers.cpp
//  deSerialKeyer
//
//  Created by nicola spesivcev on 08.11.14.
//
//

#include <stdio.h>
#include "videoPlayers.h"

videoPlayers::videoPlayers () {
    
}


videoPlayers::~videoPlayers() {
    
}


void videoPlayers::initPlayers(string path) {
    ofDirectory dir;
    dir.open(path);
    dir.allowExt("mov");
    dir.allowExt("mp4");
    
    dir.listDir();
    
    for (auto i: dir.getFiles()) {
        addVideoPlayer(i.getPocoFile().path());
    }
    
    changePlayer();

}


void videoPlayers::free () {
    for (auto i: _players) {
        delete i;
    }
}

ofxAVFVideoPlayer* videoPlayers::current() {
    return _players.at(_curPlayerIndex);
}


void videoPlayers::changePlayer() {
    current()->stop();
    
    _curPlayerIndex= ofRandom(0, _players.size() - 1);
}


void videoPlayers::addVideoPlayer (const string& path) {
    ofxAVFVideoPlayer* result = new ofxAVFVideoPlayer();
    result->loadMovie(path);
    result->setLoopState(OF_LOOP_NONE);
    
    _players.push_back(result);
}

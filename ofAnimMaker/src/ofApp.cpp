#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // connect to Teensy (hopefully)
    serial.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();

    int baud = 115200;
    bool result = serial.setup("/dev/tty.usbmodem2380891", baud);
    assert(result); // could not connect
    
    // load an asset to display
    sampler.load("images/test.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(255);
    
    
    
    /*
    
    // 288 LEDS = 864 bytes
    
    unsigned char buf[288*3];
    
    int val = 0;
    for (int i = 0; i < 288*3; ++i) {
        buf[i] = 12;
//        val++;
//        if (val > 254) val = 0;
    }
    
    serial.writeBytes(&buf[0], 288*3);
    
    ofSleepMillis(100);
     */
}

//--------------------------------------------------------------
void ofApp::draw(){
    sampler.draw(0, 0);
    
    // TODO: sample bits in images and map to serial output

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

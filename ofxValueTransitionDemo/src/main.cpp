#include "ofMain.h"
#include "ofxValueTransitionDemo.h"

int main()
{
    ofSetupOpenGL(1100, 1100, OF_WINDOW);
    ofSetWindowTitle("ofxValueTransitionDemo");
    ofSetWindowPosition(35, 35);
    ofBackground(15);

    ofRunApp(new ofxValueTransitionDemo());
}

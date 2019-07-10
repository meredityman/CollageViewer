#pragma once

#include "ofMain.h"
#include "collage.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();

        void loadCollage();
        void loadCollage(string path);


		void draw();
        void drawGui();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


        shared_ptr<collage> m_collage;

        ofParameter<bool> b_ShowTarget;
        ofParameter<bool> b_ShowSource;
        ofParameter<bool> b_Debug;
        ofParameter<float> areaWeight;

        ofxImGui::Gui gui;
};

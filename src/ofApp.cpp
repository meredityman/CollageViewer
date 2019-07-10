#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofLogToConsole();
    //ofSetLogLevel(OF_LOG_VERBOSE);

    string path = "D:/Documents/Walz Binare/AI/01_Python_ML/CollageVGG/images/collage_078/latest.svg";

    loadCollage(path);

    b_ShowTarget.set("Show Target", true);
    b_ShowSource.set("Show Source", true);
    b_Debug.set("Debug", false);
    areaWeight.set("Area Weight", 1.0, 0.0, 1.0);


    gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

void ofApp::loadCollage()
{
    auto result = ofSystemLoadDialog("Load Collage");

    
    if (result.bSuccess) {
        loadCollage(result.getPath());
    }
}

void ofApp::loadCollage(string path)
{
    m_collage = make_shared<collage>( collage(path) );

    b_ShowTarget.addListener(m_collage.get(), &collage::setShowTarget);
    b_ShowSource.addListener(m_collage.get(), &collage::setShowSource);
    b_Debug.addListener(m_collage.get(), &collage::setDebug);
    areaWeight.addListener(m_collage.get(), &collage::setAreaWeight);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofRectangle rect(0, 0, m_collage->getWidth(), m_collage->getHeight());
    ofRectangle screenRect(0, 0, ofGetWidth(), ofGetHeight());

    rect.scaleTo(screenRect, OF_SCALEMODE_FIT);

    m_collage->draw(rect.x, rect.y, rect.width, rect.height);

    drawGui();
}
//--------------------------------------------------------------
void ofApp::drawGui()
{
    auto mainSettings = ofxImGui::Settings();
	gui.begin();

	if (ofxImGui::BeginWindow("Settings", mainSettings, false))
	{

        if (ImGui::Button("Load")) {
            loadCollage();
        }

        if (ImGui::Button("Save As")) {
            m_collage->saveFile();
        }

        ofxImGui::AddParameter(b_ShowTarget);
        ofxImGui::AddParameter(b_ShowSource);
        ofxImGui::AddParameter(b_Debug);
        ofxImGui::AddParameter(areaWeight);

        if (ImGui::Button("Sort [asc]")) {
            m_collage->sortByArea(true);
        }
 
        if (ImGui::Button("Sort [desc]")) {
            m_collage->sortByArea(false);
        }

        if (ImGui::Button("Shuffle")) {
            m_collage->randomSort();
        }


	}
	ofxImGui::EndWindow(mainSettings);

	gui.end();
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

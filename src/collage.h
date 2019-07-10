#pragma once
#include "ofMain.h"


class patch {
public:

    patch(const ofXml & element, const string & path, int id);

    float getWeightedLoss(float areaWeight);
    float getArea();

    string toString();

    void draw();
    void drawDebug();


private:
    int id;
    glm::vec2 size;
    float area;
    glm::vec2 pos;
    string fullImagePath;
    ofImage img;

    string dataType;

    float loss;
    float areaAdjLoss;

};

class collage
{
public:

    collage(string path);
    ~collage();

    void draw(float x, float y, float w, float h);

    void sort();
    void sortByArea( bool asc = true);
    void randomSort();

    float getWidth() { return size.x; }
    float getHeight() { return size.y; }
    
    void saveFile();


    void setShowTarget(bool & bShowTarget);
    void setShowSource(bool & bShowSource);
    void setDebug (bool & bShowSource);
    void setAreaWeight(float & areaWeight);

private:


    void setupPatches();

    float areaWeight = 1.0;
    bool b_ShowTarget = false;
    bool b_ShowSource = true;
    bool b_Debug = false;

    vector<patch> targetPatches;
    vector<patch> sourcePatches;

    float minLoss, maxLoss;

    bool isLoaded;
    ofXml doc;
    string rootPath;

    glm::vec2 size;
    ofFbo fbo;


};


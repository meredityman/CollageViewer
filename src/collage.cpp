#include "collage.h"
#include <regex>


float easeOutQuart (float t, float b, float c, float d) {
	return -c * ((t=t/d-1)*t*t*t - 1) + b;
}

patch::patch(const ofXml & element, const string & path, int id)
{
    this->id = id;

    loss = element.getAttribute("data-loss").getFloatValue();
    size = glm::vec2(
        element.getAttribute("width").getFloatValue(),
        element.getAttribute("height").getFloatValue());

    string transform = element.getAttribute("transform").getValue();

    dataType = element.getAttribute("data-type").getValue();

   const regex re("(\\d+), (\\d+)");
    std::smatch match;
    if (regex_search(transform, match, re)) {

        float x = ofToFloat( match[1]);
        float y = ofToFloat( match[2]);
        pos = glm::vec2( x, y );
    }
    else {
        ofLogWarning() << "Could not extract position data... " << transform;
    }
    

    string imagePath = element.getAttribute("xlink:href").getValue();
    
    fullImagePath = ofFilePath::join(path, imagePath);

    img.load(fullImagePath);

    area = size.x * size.y;
    areaAdjLoss = loss / area;
}

float patch::getWeightedLoss(float areaWeight)
{

    return ofLerp( loss, areaAdjLoss, areaWeight);
}

float patch::getArea()
{
    return area;
}

string patch::toString()
{
    stringstream str;

    str << "ID: " << id << " ";
    str << "Position: (" << pos.x  << ", " << pos.y  << ") ";
    str << "Size: ("     << size.x << ", " << size.y << ") ";
    str << "Loss: " << loss << " ";
    str << "Image: " << fullImagePath << " (" << (img.isAllocated() ? "Allocated" : "Error") <<  ")\n";

    return str.str().c_str();
}

void patch::draw()
{
    img.draw(pos.x, pos.y, size.x, size.y);
}

void patch::drawDebug()
{
    
    ofSetColor(ofColor::white);
    ofDrawRectangle(pos.x, pos.y, size.x, size.y);    

    ofSetColor(ofColor::fromHsb( (  pow( 1 - areaAdjLoss, 2) ) * 255, 255, 255));
    ofDrawRectangle(pos.x + 2, pos.y + 2, size.x - 4, size.y - 4);

    ofSetColor(ofColor::white);
    ofDrawCircle(pos.x + (size.x / 2), pos.y + (size.y / 2), size.x / 4);

    ofSetColor(ofColor::fromHsb( ( 1 - loss) * 255, 255, 255));
    ofDrawCircle(pos.x + (size.x / 2), pos.y + (size.y / 2), (size.x / 4) - 2);
}


collage::collage(string path)
{
    ofFile file = ofFile(path);

    if(!file.exists()){
        isLoaded = false;
        ofLogWarning() << path << " does not exist!";
        return;
    }

    rootPath = file.getEnclosingDirectory();

    if (doc.load(file.getAbsolutePath())) {
        setupPatches();
    }
    else {
        ofLogWarning() << path << " cannot be read as XML!";
    }
}


collage::~collage()
{
}

void collage::setupPatches()
{
    auto svg = doc.getChild("svg");

    float w = svg.getAttribute("width").getFloatValue();
    float h = svg.getAttribute("height").getFloatValue();

    size = glm::vec2(w, h);

    if (w == 0 || h == 0) {
        ofLogWarning() << "Height or width not set";
        return;
    }

    targetPatches.clear();
    sourcePatches.clear();

    if (!svg.findFirst("g")) {
        ofLogWarning() << "Could not find groups";
        return;
    }


    for (auto & patchGroup : svg.getChildren("g")) {

        int id = patchGroup.getAttribute("id").getIntValue();

        if (!patchGroup.findFirst("image")) {
            ofLogWarning() << "Could not find image " << id;
            continue;
        }

        for (auto & patchEl : patchGroup.getChildren("image")) {

            string dataType = patchEl.getAttribute("data-type").getValue();

            if (dataType == "source") {
                patch newSourcePatch = patch(patchEl, rootPath, id);
                sourcePatches.emplace_back( newSourcePatch);

                ofLogVerbose() << (newSourcePatch.toString());
            }
            else if (dataType == "target") {
                patch newTargetPatch = patch(patchEl, rootPath, id);
                targetPatches.emplace_back( newTargetPatch);

                ofLogVerbose() << (newTargetPatch.toString());
            }
            else {
                ofLogWarning() << "Data type '" << dataType << "' not recognised.";
            }
        }
    }

    int sourcePatchesNum = sourcePatches.size();
    int targetPatchesNum = targetPatches.size();

    if (sourcePatchesNum != targetPatchesNum) {
        ofLogWarning() << "Number of source patched does not match number of target patches";
    }

    std::sort(targetPatches.begin(), targetPatches.end(), [=](patch a, patch b) {
        return a.getArea() > b.getArea();
    });




    fbo.allocate(size.x, size.y, GL_RGBA);
}

void collage::draw(float x, float y, float w, float h)
{
    if (!fbo.isAllocated()) return;

    fbo.begin();
    ofBackground(ofColor::white);

    if (b_ShowTarget) {

        for (auto & patch : targetPatches) {
            if (b_Debug) {
                patch.drawDebug();
            }
            else {
                patch.draw();
            }
            
        }
    }

    if (b_ShowSource) {

        for (auto & patch : sourcePatches) {
            if (b_Debug) {
                patch.drawDebug();
            }
            else {
                patch.draw();
            }            
        }
    }

    fbo.end();

    fbo.draw(x, y, w, h);

}

void collage::sort()
{
    if (sourcePatches.size() == 0) return;

    float t = easeOutQuart(areaWeight, 0.0, 1.001, 1.0);

    std::sort(sourcePatches.begin(), sourcePatches.end(), [=](patch a, patch b) {
        return a.getWeightedLoss(t) < b.getWeightedLoss(t);
    });
}

void collage::sortByArea(bool asc)
{
    if (asc) {
        std::sort(sourcePatches.begin(), sourcePatches.end(), [=](patch a, patch b) {
            return a.getArea() < b.getArea();
        });
    }
    else {
        std::sort(sourcePatches.begin(), sourcePatches.end(), [=](patch a, patch b) {
            return a.getArea() > b.getArea();
        });
    }


}

void collage::randomSort()
{
    std::random_shuffle(sourcePatches.begin(), sourcePatches.end());
}


void collage::saveFile()
{

    auto result = ofSystemSaveDialog("output", "Save as...");

    if (result.bSuccess) {
        ofPixels pixels;

        fbo.readToPixels(pixels);

        ofImage image(pixels);

        string fileName = result.filePath + ".png";
        image.save(fileName);
    }
}

void collage::setShowTarget(bool & bShowTarget)
{
    this->b_ShowTarget = bShowTarget;

}

void collage::setShowSource(bool & bShowSource)
{
    this->b_ShowSource = bShowSource;
}

void collage::setDebug(bool & bDebug)
{
    this->b_Debug = bDebug;
}

void collage::setAreaWeight(float & areaWeight)
{
    ofLogNotice() << "Sorting";
    this->areaWeight = areaWeight;
    sort();
}

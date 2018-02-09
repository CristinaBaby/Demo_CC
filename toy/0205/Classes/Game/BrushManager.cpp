//
//  BrushManager.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "BrushManager.h"
#include "AppConfigs.h"
#include "../Model/v2.0/ScribblePatternNode.h"
#include "UICpp.h"
#include "PensColorManager.h"

static BrushManager *_sharedBrush = nullptr;

BrushManager* BrushManager::getInstance()
{
    if (! _sharedBrush)
    {
        _sharedBrush = new BrushManager();
    }
    
    return _sharedBrush;
}

void BrushManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedBrush);
}

void BrushManager::init(ScribblePatternNode *pictureScribble)
{
    _pictureScribble = pictureScribble;
    _colouringLayer = (Layer*)_pictureScribble->getParent();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/stamp/Stamp0.plist");
}

BrushManager::BrushManager()
{
    ValueVector valueVector = FileUtils::getInstance()->getValueVectorFromFile("config/brushStroke.plist");
    ValueVector colorVector = FileUtils::getInstance()->getValueVectorFromFile("colors.plist");
    
    if (valueVector.empty()) {
        return;
    }
    
    if (colorVector.empty()) {
        return;
    }
    
    for (auto value = valueVector.begin(); value != valueVector.end(); value++)
    {
        auto dic = value->asValueMap();
        BrushStroke brush;
        brush.name = dic["Name"].asString();
        brush.size = SizeFromString(dic["Size"].asString());
        brush.offset = PointFromString(dic["Offset"].asString());
        
        if (dic.find("BrushIndex") != dic.end()) {
            brush.brushIndex = dic["BrushIndex"].asString();
        }
        
        if (dic.find("Density") != dic.end()) {
            brush.density = dic["Density"].asFloat();
            brush.opacity = dic["Opacity"].asFloat() * 255 / 100;
        }

        if (dic.find("StepMax") != dic.end()) {
            brush.solid = false;
            brush.stepMax = dic["StepMax"].asFloat();
            brush.stepMin = dic["StepMin"].asFloat();
        }
        
        if (dic.find("Color") != dic.end()) {
            brush.color = dic["Color"].asBool();
        }
        
        if (dic.find("Select") != dic.end()) {
            brush.select = dic["Select"].asBool();
        }
        
        if (dic.find("OriginalColor") != dic.end()) {
            brush.originalColor = getColorFromString(dic["OriginalColor"].asString());
        }
        
        if (dic.find("Changes") != dic.end()) {
            brush.changes = getChangesFromString(dic["Changes"].asString());
        }
        
        _stroke.push_back(brush);
    }
    
    for (auto cava : colorVector) {
        auto color = cava.asString();
        _colors.push_back(getColorFromString(color));
    }
    
    for (int i = 0; i < 12; i++) {
        _goldpenColors.push_back(_colors.at(_goldpenColorIndex[i]));
    }
    
    for (int i = 0; i < 12; i++) {
        _patternSelects.push_back(string("game/pattern_select/pattern_sel" + std::to_string(i + 1) +  ".png"));
    }
}

Color4F BrushManager::getColorFromString(std::string str){
    int color4F[3];
    auto index = str.find(" ");
    string s1 = str.substr(0,index);
    color4F[0] = atoi(s1.c_str());
    
    string s2 = str.substr(index + 1,str.length());
    index = s2.find(" ");
    string s3 = s2.substr(0,index);
    color4F[1] = atoi(s3.c_str());
    
    string s4 = s2.substr(index + 1, s2.length());
    color4F[2] = atoi(s4.c_str());
    
    return Color4F(color4F[0]/255.0, color4F[1]/255.0, color4F[2]/255.0, 1);
}

vector<string> BrushManager::getChangesFromString(std::string str){
    vector<string> changes;
    
    auto index = str.find(" ");
    string s1 = str.substr(0,index);
    changes.push_back(s1);
    
    string s2 = str.substr(index + 1,str.length());
    index = s2.find(" ");
    string s3 = s2.substr(0,index);
    changes.push_back(s3);
    
    string s4 = s2.substr(index + 1, s2.length());
    changes.push_back(s4);
    
    return changes;
}

//貌似频繁地使用这个方法会crash, 只在换笔的时候调用这个函数
void BrushManager::useBrush(int index)
{
    //判断是否和上次一样, 避免频繁使用useBrush
    string brushName = getBrushName();
    if (index == _currentIndex && brushName == _lastBrushName) {
        return;
    }
    
    auto brush = _stroke.at(index);
    _currentIndex = index;
    
    CC_SAFE_RELEASE_NULL(_stamp);
    
    if(brush.name != "rollerbrush" && brush.name != "seal" && brush.name != "gold_pen_body")
    {
        _pictureScribble->useBrush("game/brushstroke/" + brush.brushIndex + "/" + brush.brushIndex + "-" + brushName + ".png");
        _pictureScribble->setDensity(brush.density);
        _pictureScribble->setOpacity(brush.opacity);
        _pictureScribble->setSolid(brush.solid);
        _pictureScribble->setStepMax(brush.stepMax);
        _pictureScribble->setStepMin(brush.stepMin);
        
        _lastBrushName = brushName;
        
        if (xBrush->getBrush().color)
        {
            setColor(_lastColorsIndex);
        }
    }
    else
    {
        _pictureScribble->useBrush("game/brushstroke/" + brush.brushIndex + "/" + brush.brushIndex + "-" + brushName + ".png");
        _pictureScribble->setOpacity(brush.opacity);
        
        if(brush.name == "seal")
        {
            _stamp = Sprite::createWithSpriteFrameName("stamp" + std::to_string(quickRandom(1, 10)) +  ".png");
            CC_SAFE_RETAIN(_stamp);
        }
        else if(brush.name == "rollerbrush")
        {
            setColor(brush.chooseIndex);
        }
        else if(brush.name == "gold_pen_body")
        {
            setColor(brush.chooseIndex);
        }
        
        _lastBrushName.clear();
    }
    
    if (brush.name == "eraser") {
        _pictureScribble->useTarget(Color4F(255/255, 255/255, 255/255, 1));
    }
    else if (brush.name == "rainbowpen")
    {
        _pictureScribble->useTarget(PensColorManager::getColorManager()->getUseColor());
    }
}

void BrushManager::updateBrush()
{
    useBrush(_currentIndex);
}

void BrushManager::setChooseIndex(int index)
{
    auto name = getBrush().name;
    if (name == "gold_pen_body" || name == "rollerbrush") {
        _stroke.at(_currentIndex).chooseIndex = index;
    }
    else
    {
        _generalChooseIndex = index;
    }
}

int BrushManager::getChooseIndex() {
    
    auto name = getBrush().name;
    if (name == "gold_pen_body" || name == "rollerbrush") {
        return getBrush().chooseIndex;
    }
    else
    {
        return _generalChooseIndex;
    }
}

string BrushManager::getBrushName()
{
    float scale = 1/_colouringLayer->getScale();
    
    string ret;
    
    if (scale > 0.95) {
        ret = "1";
    }
    else if (scale > 0.85)
    {
        ret = "0.9";
    }
    else if (scale > 0.75)
    {
        ret = "0.8";
    }
    else if (scale > 0.65)
    {
       ret = "0.7";
    }
    else if (scale > 0.55)
    {
        ret = "0.6";
    }
    else if (scale > 0.45)
    {
        ret = "0.5";
    }
    else if (scale > 0.35)
    {
        ret = "0.4";
    }
    else
    {
        ret = "0.3";
    }
    
    return std::move(ret);
}

const vector<Color4F> & BrushManager::getColors()
{
    return _colors;
}

const vector<Color4F> & BrushManager::getGoldPenColors()
{
    return _goldpenColors;
}

const vector<string> & BrushManager::getPatternSelects()
{
    return _patternSelects;
}

Color4F BrushManager::getOrigianlColor(int index)
{
    if(_stroke.at(index).color)
    {
        return _stroke.at(index).originalColor;
    }
    else
    {
        return Color4F(1,1,1,1); // no use
    }
}

const BrushManager::BrushStroke & BrushManager::getBrush()
{
    return _stroke.at(_currentIndex);
}

const BrushManager::BrushStroke & BrushManager::getBrush(int index)
{
    return _stroke.at(index);
}

bool BrushManager::useSuggestColor(const Color3B &color)
{
    auto brush = getBrush();
    
    if(brush.name == "seal" || brush.name == "rollerbrush" || brush.name == "gold_pen_body"
       || brush.name == "eraser" || brush.name == "rainbowpen")
    {
        return false;
    }
    
    if(brush.color == false)
    {
        return false;
        
        while (brush.color == false) {
            _currentIndex = quickRandom(0, _stroke.size() - 1);
            brush = _stroke.at(_currentIndex);
        }
        
        useBrush(_currentIndex);
    }
    
    _pictureScribble->useTarget(Color4F(color));
    
    return true;
}

void BrushManager::setColor(int colorsIndex)
{
    if(getBrush().name == "gold_pen_body")
    {
        Sprite *patternSprite = Sprite::create("game/goldpen/goldpen" + std::to_string(colorsIndex + 1) + ".png");
        _pictureScribble->useTarget(patternSprite);
    }
    else if(getBrush().name == "rollerbrush")
    {
        Sprite *patternSprite = Sprite::create("game/pattern/pattern" + std::to_string(colorsIndex + 1) + ".png");
        _pictureScribble->useTarget(patternSprite);
    }
    else
    {
        _lastColorsIndex = colorsIndex;
        _pictureScribble->useTarget(xBrush->getColors().at(colorsIndex));
    }
}

void BrushManager::clear()
{
    _currentIndex = 0;
    _lastBrushName.clear();
    
    for (auto brush : _stroke) {
        brush.chooseIndex = 0;
    }
}

Sprite * BrushManager::getStamp(bool bTest)
{
    if(_stamp != nullptr && bTest == false)
    {
        _stamp->setSpriteFrame("stamp" + std::to_string(quickRandom(1, 10)) +  ".png");
        
        int stampColorIndex = quickRandom(0, _colors.size() - 1);
        while (_lastStampColorIndex == stampColorIndex) {
            stampColorIndex = quickRandom(0, _colors.size() - 1);
        }
        _lastStampColorIndex = stampColorIndex;
        
        auto color = _colors.at(_lastStampColorIndex);
        _stamp->setColor(Color3B(color));
    }
    
    return _stamp;
}



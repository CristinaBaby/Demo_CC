//
//  PensColorManager.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/17.
//
//

#include "PensColorManager.h"
#include "UITools.h"
#include "PensManager.h"
#include "LockManager.h"

#define COLOR_COUNT             (10)

PensColorManager* PensColorManager::msg = nullptr;

PensColorManager* PensColorManager::getColorManager(){
    if (!msg) {
        msg = new (std::nothrow)PensColorManager();
        msg->initColor();
    }
    return msg;
}

PensColorManager::PensColorManager(){
    auto root = FileUtils::getInstance()->getValueMapFromFile("colors.plist");
    _colorVector = root["Pens"].asValueVector();
    
    drawType = true;
}

void PensColorManager::clear()
{
    drawType = true;
    _rainbowPen = false;
}

bool PensColorManager::initColor(){
    
    Color color[COLOR_COUNT] = {
        Color{230,0,18},
        Color{234,85,32},
        Color{242,142,0},
        Color{254,209,0},
        Color{195,215,0},
        Color{21,173,103},
        Color{35,173,229},
        Color{19,64,152},
        Color{115,91,158},
        Color{111,24,110}};
    Color offset[COLOR_COUNT] = {
        Color{4 ,85, 14},
        Color{8, 57, -32},
        Color{12 ,67, 0},
        Color{-59 ,6, 0},
        Color{-174, -42, 0},
        Color{14, 0, 126},
        Color{-16, -107, -77},
        Color{96 ,27, 6},
        Color{-4 ,-67 ,-48},
        Color{119 ,-24, -92}};
    
    for (int i = 0; i<COLOR_COUNT; i++) {
        PensColor penscolor;
        Color4F offset4f = Color4F(offset[i].red / 255, offset[i].green / 255, offset[i].blue / 255, 1);
        Color4F color4f = Color4F(color[i].red / 255, color[i].green / 255, color[i].blue / 255, 1);
        penscolor.original = color4f;
        penscolor.offset = offset4f;
        _pensColor.push_back(penscolor);
    }
    
    _color = Color4F(color[0].red / 255, color[0].green / 255, color[0].blue / 255, 1);
    
    // fill mode
    
    int start = 24;
    for (int index = 0; index < 6; index ++) {
        if(xLock->isLocked(index))
        {
            start +=5;
            continue;
        }
        for(int acc = 0; acc < 5; acc++)
        {
            _unLockPens.push_back(start + acc);
        }
        start +=5;
    }
    
    for(int index = 0; index < 24; index++){
        _unLockPens.push_back(index);
    }
    
    for (int i = 0; i < 54; i++) {
        
        if (i == 0) {
            _pensColorForFill.push_back(Color4F::WHITE);
        }
        else
        {
            Color4F color;
            color.r = _colorVector.at(i).asValueMap()["R"].asFloat()/255;
            color.g = _colorVector.at(i).asValueMap()["G"].asFloat()/255;
            color.b = _colorVector.at(i).asValueMap()["B"].asFloat()/255;
            color.a = 1;
            
            _pensColorForFill.push_back(color);
        }
    }
    
    return true;
}

Color4F PensColorManager::getUseColor(){
    return _color;
}

Color4F PensColorManager::getColor()
{
    _acc++;
    
    if (_acc >= 5) {
        _index++;
        _acc = 0;
        if (_index == COLOR_COUNT) {
            _index = 0;
        }
        // 取出 纯色
        _color = _pensColor.at(_index).original;
        return _color;
    }
    _color.r = _color.r + _pensColor.at(_index).offset.r / 5.0;
    _color.g = _color.g + _pensColor.at(_index).offset.g / 5.0;
    _color.b = _color.b + _pensColor.at(_index).offset.b / 5.0;
    log ("acc = %d", _acc);
    return _color;
    // 取出 渐变色
}

Color4F PensColorManager::getSugarColor(){
    Color4F color = _pensColor.at(_tag).original;
    _tag++;
    if (_tag > 9) {
        _tag = 0;
    }
    return color;
}

Color4F PensColorManager::getColorForFillMode(){
    int size = _unLockPens.size();
    int rand_num = quickRandom(1, size - 1);
    log("----------fillType rand_num-------- = %d",rand_num);
    Color4F color = _pensColorForFill.at(rand_num);
    return color;
}

Color4F PensColorManager::getColorForInitPens() {
    Color4F color = _pensColorForFill.at(_step);
    _step++;
    if(_step == 54)
    {
        _step = 0;
    }
    
    return color;
}


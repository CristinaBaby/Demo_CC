//
//  BrushManager.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include <cocos2d.h>
#include "UITools.h"

class ScribblePatternNode;

#define xBrush              (BrushManager::getInstance())

class BrushManager {
    
public:
    struct BrushStroke
    {
        string name;
        float density;
        float opacity = 255;
        bool solid = true;
        float stepMax;
        float stepMin;
        bool color = false;
        bool select = false;
        Color4F originalColor;
        vector<string> changes;
        Size size;
        Vec2 offset;
        string brushIndex = "1";
        
        int chooseIndex = 0;
    };
    
    
    static BrushManager* getInstance();
    void destroyInstance();
    
    void init(ScribblePatternNode *pictureScribble);
    void useBrush(int index);
    void updateBrush();
    
    const BrushStroke & getBrush();
    const BrushStroke & getBrush(int index);
    
    const vector<Color4F> & getColors();
    const vector<Color4F> & getGoldPenColors();
    const vector<string> & getPatternSelects();
    bool useSuggestColor(const Color3B &color);

    int getChooseIndex();
    void setChooseIndex(int index);

	Color4F getOrigianlColor(int index);
    
    void setColor(int colorsIndex);
    
    Sprite * getStamp(bool bTest = false);
    
    void clear();

private:
    BrushManager();
    string getBrushName();
    Color4F getColorFromString(std::string str);
    vector<string> getChangesFromString(std::string str);
    
private:
    vector<BrushStroke> _stroke;
    vector<Color4F> _colors;
	vector<Color4F> _origin;
    vector<Color4F> _goldpenColors;
    vector<string>  _patternSelects;
    
    ScribblePatternNode * _pictureScribble;
    Layer               * _colouringLayer;
    
    int     _currentIndex    = 0;
    string  _lastBrushName;
    
    int     _lastColorsIndex = 0;                   //通用颜色
    int     _generalChooseIndex = 0;                //最后选中的index, 仅仅是对当前笔有效
    Sprite* _stamp = nullptr;
    int     _lastStampColorIndex = 0;
    
    int     _goldpenColorIndex[12] = {0, 2, 6, 7, 10, 12, 14, 16, 18, 19, 20, 21};
};

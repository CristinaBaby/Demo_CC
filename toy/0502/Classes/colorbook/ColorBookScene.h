//
//  ColorBookScene.h
//  Bedtime Story Mermaid
//
//  Created by Luo Xiaopeng.
//
//
#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"

class ScribblePatternNode;
class ColorPenSprite;
class PatternPenSprite;


class ColorBookScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ColorBookScene);
    ~ColorBookScene();
    
    bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void touchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void initTouches();
    void initPattern();
    void initWireframe();
    
    void generateCurrentPaintingPicture(const std::string& pictureName);
    void onCheckBox(Ref* ,CheckBox::EventType type);
    void changePen(ValueMap &vm);
    void useEraser();
    void changePattern(const string& name);
    
    void  savePicture(const std::string& pictureName);
    void finishSaveImage();
    
    void getImagePath();
    int getPackImageCount(int pack);
    void disableAllButtons();
    
protected:
    virtual void onExit() override;
    
private:
    void onButton(Ref* sender, Widget::TouchEventType type);
    
    LayerColor * _loadingLayer;
    
    bool _finishLoadHistory;
    bool _finishLoadPicture;
    bool _finishLoadMask;
    
    ScribblePatternNode *_pictureScribble;
    Sprite *_picture;
    Sprite *_mask;
    Image *_patternImage;
    
    string _picturePath = "L01/L01_picture.png";
    string _pictureMaskPath = "L01/L01_mask.png";
    string _appendHistoryPath = "L01_history.png";
    string _pictureHistoryPath;
    
    cocos2d::Layer *_colouringLayer;
    
    CheckBox*   _chkPen;
    CheckBox*   _chkMorePen;
    CheckBox*   _chkPattern;
    
    Layout*     _layoutPen;
    Layout*     _layoutMorePen;
    Layout*     _layoutPattern;
    
    
    Layout*     _popup;
    
    ValueVector _pen1;
    ValueVector _pen2;
    ValueVector *_pen;
    
    vector<Button*> _btnPen;
    vector<Vec2>    _btnPos;
    
    ImageView*  _image;
    Image *     _pictureImage;
    
    Layout *    _root;
    
    //历史记录相关
    bool _continue = false;
    void onProgress();
    void onDialog(const string& name);
    bool _hasPaint = false;
    
    
    //切换画笔相关
    Vec2        _oldChoosePos;
    Widget *    _chooseBtn = nullptr;
    
    //画笔音效
    unsigned int _nSoundId = 0;
};


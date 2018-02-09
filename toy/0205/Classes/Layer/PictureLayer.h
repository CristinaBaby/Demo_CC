//
//  PictureLayer.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include <cocos2d.h>
#include "UITools.h"

class  PictureLayer : public Layer
{
public :
    static PictureLayer* create(const int index, const int count);
    
    void onButton(Ref* pSender, Widget::TouchEventType type);

    void setPackIndex(int index ,int count);
    
    void loadIofo(const int pageIndex, int times = 1);
    
    void turnTopage(const string& name);
    
    void nextScene(int pageIndex);
    
    void turnPageAction(const string& name);
    
    void setTouchEnable(bool touch);
    
    void initPageData();
    
    bool isPictrueExit(const string& pictureName);
        
    void setPackIndex(int packIndex) { _packIndex = packIndex;};
    
private:
    virtual bool init(const int index, const int count);
    PictureLayer();

private:
    int _packIndex;
    
    vector<Button*> _categorys;
    
    int _pageIndex;
    int _categorysIndex;
    int _pageCount;
    int _count;
    int _loadIndex;
    
    Widget* _root;
    
    Sprite* tempLeft;
    Sprite* tempRight;
    ImageView* book;
    Button* _preBtn;
    Button* _nextBtn;
    Button* _backBtn;;
    ParticleSystemQuad* _particle;
    
    bool                _blockAll = false;
};
#pragma once

#include "../utilities/UIHeader.h"
#include "ui/UIListView.h"

#define T_TILE_SPRITE           (10)

class JPSideBar : public ListView{
    
public:
    static  JPSideBar* create(float scale , float separationSpace, Layout *root);
    
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent);
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent);
    
    void setEndScale(float endScale);
    
public:
    bool isSurrportZoom = false;
    float zoomScale = 1.0;
    bool isDragContent = false;     //是否选中滑动条内的子节点
    float separationSpace;          //start 对 start的间距
    float topDistrict;
    
private:
    Node *curSelectNode = nullptr;            //当前选择的那个tile
    Vec2 beginPoint;
    float beginScale = 0;
    Layout *_root;
    float endScale = 0.5f;
};

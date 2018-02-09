
#ifndef __AddIcingScene__
#define __AddIcingScene__


#include "ExtensionScene.h"
#include<cmath>
#include "CircularScrollView.h"
#include "ChooseLayer.h"

class Ellipse {
    
    
public:
    int a;
    int b;
    int segment;
    Vec2 center;
    
    void init(int x,int y,Vec2 po,int se){
        a = x;
        b = y;
        center = po;
        segment = se;
    }
    
    
    inline bool containPoint(Vec2 position){
        Vec2 po = Vec2(center,position);
        float temp = pow(po.x,2)/pow(a,2)+pow(po.y,2)/pow(b,2);
        if(temp<1.0f){
            return true;
        }
        return false;
    }
    
    inline Vec3 getSegmentPosition(Vec2 position){
        Vec2 po = Vec2(center,position);

        int segmentIndex = -1;
        if(containPoint(position)){
            int count = segment/2;
            float longth = a*2;
            float segmentLongth = longth/count;
            segmentIndex = (po.x+a)/segmentLongth;
            float x = segmentIndex*segmentLongth-a;
            float yy = (1.0f-pow(x,2)/pow(a,2))*pow(b,2);
            float y = sqrt(abs(yy));
            if(position.y<center.y+1){
                y = -y;
            }else{
                segmentIndex = segment-segmentIndex-1;
            }
            return Vec3(x+center.x, y+center.y,segmentIndex);
        }
        return Vec3::ZERO;
    }
    inline int getSegmentIndex(Vec2 position){
        Vec2 po = Vec2(center,position);
        int segmentIndex = -1;
        if(containPoint(position)){
            int count = segment/2;
            float longth = a*2;
            float segmentLongth = longth/count;
            segmentIndex = (po.x+a)/segmentLongth;
            if(position.y>=center.y){
                segmentIndex = segment-segmentIndex-1;
            }
        }
        return segmentIndex;
    }
};

class AddIcingScene : public ExtensionScene
{
public:
    AddIcingScene();
    ~AddIcingScene();
    
    MY_SCENE(AddIcingScene);
    CREATE_FUNC(AddIcingScene);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onButtonCallback(Button* btn);
    
    RenderTexture* getResultRender();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
    
    virtual void onShopItemBuy(cocos2d::Ref *pRef);
protected:
    virtual void _initData();
    void _reset();
    
    void _showChooseIcing();
    void _showColorScrollView();
    
    void _onIcingCallback(int index);
    void _onColorItemCallback(Node*);
    
    Sprite* m_pStand;
    Sprite* m_pCake;
    Sprite* m_pFrosting;
    Layer* m_pAcingLayer;
    
    DragNode* m_pTool;
    
    Ellipse m_EllipseUpOutter;
    Ellipse m_EllipseUpInner;
    Ellipse m_EllipseDownOutter;
    Ellipse m_EllipseDownInner;
    
    IcingHSB m_IcingHSB;
    //UI
    CircularScrollView* m_pColorScrollview;
    GuideLayer *m_pGuideLayer2;
    
    Sprite* m_pDarkIcing;
    Sprite* m_pBrightIcing;
    
    int m_nCurType;
    int m_nCurIndex;
    
    int m_nSegment;
    bool m_flag[50];
};

#endif
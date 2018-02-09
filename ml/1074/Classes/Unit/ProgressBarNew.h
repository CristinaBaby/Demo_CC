//
//  ProgressBarNew.h
//  Make
//
//  Created by dengcheng on 15/10/23.
//
//

#ifndef ProgressBarNew_hpp
#define ProgressBarNew_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class ProgressBarNew:public Node {
    
public:
    typedef enum {
        eStateNormal,
        eStateOK,
        eStateBad,
        eStateOver,
    } ProgressBarNewStatus;
    enum class Direction{
        eDirectionV, //垂直
        eDirectionH, //水平
    };
public:
    ProgressBarNew();
    static ProgressBarNew* create(std::string barPath,std::string bgPath,std::string maskPath,Direction dir = Direction::eDirectionH);
    bool init(std::string barPath,std::string badPath,std::string maskPath,Direction dir = Direction::eDirectionH);
    void setFlag(float ok,float bad,int segment0,int segment1,int segment2);
    void setColorSegment(Color3B c1,Color3B c2,Color3B c3){
        m_c0 = c1;
        m_c1 = c2;
        m_c2 = c3;
    }
    void beginProgress(float duration);
    void stopProgress(){unscheduleUpdate();};
    Direction getDirection(){return m_eDirection;}
    std::function<void(ProgressBarNew*,int)> progressNewState;
    
    void doSchedule();
    void pauseProgress();
    void resumeProgress();
    
    Node* targetNode;
protected:
    void update(float dt);
private:
    
    Direction m_eDirection;
    
    ClippingNode* m_pClipping;
    Sprite* m_pBar;
    Sprite* m_pBarBg;

    float m_fv;
    float m_fLineOK;
    float m_fLineBad;
    int m_nSegment_0;
    int m_nSegment_1;
    int m_nSegment_2;
    Color3B m_c0;
    Color3B m_c1;
    Color3B m_c2;
};
#endif /* ProgressBarNew_hpp */
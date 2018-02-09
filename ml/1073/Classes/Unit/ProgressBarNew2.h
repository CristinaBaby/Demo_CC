//
//  ProgressBarNew2.h
//  Make
//
//  Created by dengcheng on 15/10/23.
//
//

#ifndef ProgressBarNew2_hpp
#define ProgressBarNew2_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class ProgressBarNew2:public Node {
    
public:
    typedef enum {
        eStateNormal,
        eStateOK,
        eStateBad,
        eStateOver,
    } ProgressBarNew2Status;
    enum class Direction{
        eDirectionV, //垂直
        eDirectionH, //水平
    };
public:
    ProgressBarNew2();
    static ProgressBarNew2* create(std::string barPath,std::string bgPath,std::string maskPath,Direction dir = Direction::eDirectionH);
    bool init(std::string barPath,std::string badPath,std::string maskPath,Direction dir = Direction::eDirectionH);
    void setFlag(float ok,float bad,int segment0,int segment1,int segment2);
    
    void beginProgress(float duration);
    void stopProgress(){unscheduleUpdate();};
    Direction getDirection(){return m_eDirection;}
    std::function<void(ProgressBarNew2*,int)> progressNewState;
    
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
};
#endif /* ProgressBarNew2_hpp */
//
//  ProgressBar.h
//  Make
//
//  Created by dengcheng on 15/10/23.
//
//

#ifndef ProgressBar_hpp
#define ProgressBar_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;


class ProgressBar:public Node {
    
public:
    typedef enum {
        eStateNormal,
        eStateOK,
        eStateBad,
        eStateOver,
    } ProgressBarStatus;
    enum class Direction{
        eDirectionV, //垂直
        eDirectionH, //水平
    };
public:
    ProgressBar();
    static ProgressBar* create(std::string barPath,std::string beginPath,std::string okPath,std::string badPath,std::string maskPath,Direction dir = Direction::eDirectionH);
    bool init(std::string barPath,std::string beginPath,std::string okPath,std::string badPath,std::string maskPath,Direction dir = Direction::eDirectionH);
    void setPercentStage(int begin,int ok,int bad);
    void beginProgress(float duration);
    void stopProgress(){unscheduleUpdate();};
    Direction getDirection(){return m_eDirection;}
    std::function<void(int)> progressNewState;
    
    void doSchedule();
    void pauseProgress();
    void resumeProgress();
protected:
    void update(float dt);
private:
    std::string m_sBarPath;
    std::string m_sBeginPath;
    std::string m_sOkPath;
    std::string m_s_BadPath;
    
    Direction m_eDirection;
    
    int m_nBegin;
    int m_nOk;
    int m_nBad;
    ClippingNode* m_pClippingNode;
    Sprite* m_pBar;
    Sprite* m_pHead;
    float m_fv;
};
#endif /* ProgressBar_hpp */
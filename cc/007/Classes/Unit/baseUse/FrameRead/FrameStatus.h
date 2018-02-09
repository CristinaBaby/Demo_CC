//
//  FrameRead.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/2/12.
//
//

#ifndef FrameRead_hpp
#define FrameRead_hpp
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
class FrameStatus :public cocos2d::Ref {
    CC_SYNTHESIZE_RETAIN(cocostudio::timeline::ActionTimeline*, _searchAction, SearchAction);
public:
    FrameStatus();
    virtual ~FrameStatus();
    static FrameStatus*create(const std::string&nodeName);
     cocos2d::Vector<cocostudio::timeline::Frame*> searchFrame(const std::string& animationName);
    cocos2d::ActionInterval* createAction(float time,const std::string& animationName);
    void applayAnimation(const std::string& animationName);
    void remove();
    cocos2d::Vec2 searchPostion(const std::string& animationName);
protected:
    bool init(const std::string& nodeName);
protected:
    std::string nodeName;
    std::map<std::string,cocos2d::Vector<cocostudio::timeline::Frame*>> caches;
    static const cocos2d::Vector<cocostudio::timeline::Frame*> zero;
};

#endif /* FrameRead_hpp */

//
//  AddSprinkle.h
//  DoutCakeMaker
//
//  Created by luotianqiang1 on 16/8/8.
//
//

#ifndef AddSprinkle_hpp
#define AddSprinkle_hpp

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class AddSprinkle : public Node{
public:
    CREATE_FUNC(AddSprinkle)
    void setSprinklePath(const vector<string>&);
    void caculatePos();
    void caculatePos(const Size& parentSize);
    void setEnable(bool);
    void setSwallowTouches(bool);
protected:
    bool init();
    void onEnter();
    bool touchBegan(Touch*,Event*);
    void touchEnded(Touch*,Event*);
protected:
    bool enable,isSwallowTouches;
    EventListenerTouchOneByOne* lis;
    vector<string> sprinklePath;
    vector<int> maxYs, minYs;
    Size parentSize;
public:
    std::function<void(Touch*)> touchCallBack;
};
#endif /* AddSprinkle_hpp */

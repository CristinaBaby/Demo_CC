//
//  MixPoint.h
//  candyfactory
//
//  Created by luotianqiang1 on 16/4/25.
//
//

#ifndef MixPoint_hpp
#define MixPoint_hpp

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class MixPoint: public Node{
public:
    CREATE_FUNC(MixPoint);
    void setPointPath(const string&);
    void run(float time);
    void stop();
protected:
    void updateTime(float);
    void crateFunc();
protected:
    float currentTime;
    MixPoint();
    bool init();
    string pointPath;
    float distanceToCenter;
    float delayTime;
    int acTionTag;
};
#endif /* MixPoint_hpp */

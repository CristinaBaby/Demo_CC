
#ifndef MixPoint_h
#define MixPoint_h

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class MixPoint: public Node{
public:
    CREATE_FUNC(MixPoint);
    void setPointPath(const string&);
    void run(float time);
    void stop();
    void countMove(float dt);
    void stopCountMove();
    std::function<void()> countMoveCallback;
protected:
    void updateTime(float);
    void crateFunc();
    void updateCount(float);
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

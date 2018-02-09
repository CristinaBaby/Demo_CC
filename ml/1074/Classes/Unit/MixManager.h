
#ifndef __MixManager__
#define __MixManager__


#include "SingleTon.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class MixManager : public SingleTon<MixManager>
{
public:
    void mix();
    void startStir();
    void endStir();
    void saveMixture();
    
    void reset(){
        _mixtureStartIndex = 0;
        _mixCount = 0;
        _mixSpeed = 0;
        _mixRotateSpeed = 0;
        _mixturePathEX = "";
        _mixtureNode = nullptr;
        onMixFinish = nullptr;
        _count = 0;
        _scheduleSegment = 4;
        _mixturePos = Vec2::ZERO;
        _mixtureSize = Size::ZERO;
    }
    int _mixtureStartIndex;
    int _mixCount;
    int _mixSpeed;
    int _mixRotateSpeed;
    int _scheduleSegment = 4;
    Size _mixtureSize;
    Vec2 _mixturePos;
    std::string _mixturePathEX;
    Node* _mixtureNode;
    
    std::function<void()> onMixFinish = nullptr;
    std::function<void(int step)> onMixCallback = nullptr;
    std::function<void(int step)> onScheduleCallback = nullptr;
protected:
    int _count;
};

#endif
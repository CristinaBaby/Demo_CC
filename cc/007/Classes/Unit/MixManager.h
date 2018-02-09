
#ifndef __MixManager__
#define __MixManager__


#include "SingleTon.h"
#include "cocos2d.h"

using namespace std;

USING_NS_CC;
using namespace cocos2d::ui;
class MixManager : public SingleTon<MixManager>
{
public:
    void addPoint(Vec2 pos);
//    void caculator();
protected:
    std::vector<Vec2> _pointVector;
};

#endif
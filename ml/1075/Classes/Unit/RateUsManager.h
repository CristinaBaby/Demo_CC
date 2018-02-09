
#ifndef __RateUsManager__
#define __RateUsManager__


#include "SingleTon.h"
#include "cocos2d.h"

using namespace std;

class RateUsManager : public SingleTon<RateUsManager>
{
public:
    void showRateUs(bool check = true);
    void showRateUsWhy();
    std::function<void(int)>onRateusCallback;
protected:
    
};

#endif
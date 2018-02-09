
#ifndef StickFoodHelp_h
#define StickFoodHelp_h

#include <stdio.h>
#include "Heads.h"

class StickFoodHelp
{
public:
    StickFoodHelp();
    ~StickFoodHelp();
    
    static Sprite* createStick(vector<string> dataVector,bool roasted = true);
    static vector<Sprite*> createBBQMeat(vector<string> dataVector,bool roasted = true);
};
#endif /* StickFoodHelp_h */

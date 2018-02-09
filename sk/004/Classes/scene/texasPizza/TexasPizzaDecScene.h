//
//  TexasPizzaDecScene.h
//  shake004
//
//  Created by liji on 16/10/27.
//
//

#ifndef TexasPizzaDecScene_h
#define TexasPizzaDecScene_h

#include <stdio.h>
#include "DecorationBaseLayer.h"
#include "ESMakeBaseScene.h"
#include "TexasPizzaNode.h"
using namespace std;
class ComponentTouchMove;
class TexasPizzaDecScene : public DecorationBaseLayer
{
public:
    MY_SCENE(TexasPizzaDecScene);
    CREATE_FUNC(TexasPizzaDecScene);
    virtual bool init() override;
    TexasPizzaDecScene();
    ~TexasPizzaDecScene(){};
    
protected:
    TexasPizzaNode* _texasPizze;
    bool _firstSticker = true;
    bool _firstCup = false;
    bool _firstStraw = true;
    bool _firstBg = true;
private:
    void showScrollView(int index);
    void onEnter();
    void setTypeEnable(bool enable);
};

#endif /* TexasPizzaDecScene_h */

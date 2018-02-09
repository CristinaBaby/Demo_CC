
#ifndef __FriedUnit__
#define __FriedUnit__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DragNode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class FriedUnit : public Node
{
public:
    FriedUnit();
    ~FriedUnit();
    
    CREATE_FUNC(FriedUnit);
    
    virtual bool init();
    void setFood(std::string path);
    void setOil(std::string path);
    void addParticle(std::string path);
    void doFry();
    void stopFry();
    Sprite* getFood(){
        return _pFood;
    }
    
    Rect getRectWorld();
    bool isFriedOK;
    int flag;
protected:
    Sprite* _pOil;
    Sprite* _pFood;
    std::vector<ParticleSystemQuad*> _oilParticles;
};


#endif
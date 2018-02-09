
#include "FriedUnit.h"
#include "Global.h"
#include "AudioHelp.h"

USING_NS_CC;

FriedUnit::FriedUnit()
{
    _pOil = nullptr;
    _pFood = nullptr;
    isFriedOK = false;
    flag = 0;
}

FriedUnit::~FriedUnit()
{
    
}
bool FriedUnit::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    return true;
}

void FriedUnit::setFood(std::string path)
{
    if (!_pFood) {
        _pFood = Sprite::create(path);
        this->addChild(_pFood,2);
    }else{
        _pFood->setTexture(path);
    }
}
void FriedUnit::setOil(std::string path)
{
    if (!_pOil) {
        _pOil = Sprite::create(path);
        this->addChild(_pOil);
    }else{
        _pOil->setTexture(path);
    }
    _pOil->setOpacity(0);
}
void FriedUnit::addParticle(std::string path)
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create(path);
    this->addChild(pParticle,5);
    _oilParticles.push_back(pParticle);
    pParticle->setPosition(Vec2::ZERO);
    pParticle->setVisible(false);
}

void FriedUnit::doFry()
{
    AudioHelp::getInstance()->playLoopEffect("frying_food.mp3");
    if(_pOil){
        _pOil->runAction(FadeIn::create(3));
    }
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        for(auto particle:_oilParticles){
            particle->setVisible(true);
            particle->resetSystem();
        }
    }), NULL));
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(1, 1)),
                                                           MoveBy::create(0.1, Vec2(-1, -1)), NULL)));
}
void FriedUnit::stopFry()
{
    AudioHelp::getInstance()->stopLoopEffect();
    this->stopAllActions();
    if(_pOil){
        _pOil->runAction(FadeOut::create(0.5));
    }
    
    for(auto particle:_oilParticles){
        particle->stopSystem();
    }
}

Rect FriedUnit::getRectWorld()
{
    if(!_pFood){
        return Rect::ZERO;
    }
    Rect rect = _pFood->getBoundingBox();
    rect.origin = this->convertToWorldSpace(rect.origin);
    return rect;
    
}